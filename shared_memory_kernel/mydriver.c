#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "mydriver.h"

int share_major = SHARE_MAJOR;  //主设备号
int share_minor = 0;    //次设备号
module_param(share_major, int, S_IRUGO);
module_param(share_minor, int, S_IRUGO);

struct share_dev *share_device; //设备的数据结构

int share_cleanmem(struct share_dev *dev){
    if (dev){
        if (dev->data){
            kfree(dev->data);
        }
        dev->data = NULL;
        dev->size = 0;
    }

    return 0;
}

//open函数，设置inode和file结构中的相关内容，成功返回0
int share_open(struct inode *inode, struct file *filp)
{
    struct share_dev *dev;

    dev = container_of(inode->i_cdev, struct share_dev, cdev);  //获取包含cdev结构的share_dev结构
    filp->private_data = dev;   //将share_dev结构嵌入到file结构中

    //如果文件不可读，则撤销
    if ((filp->f_flags & O_ACCMODE) == O_WRONLY){
        //申请进入临界区
        if (down_interruptible(&dev->sem)){
            return -ERESTARTSYS;
        }

        share_cleanmem(dev)
        up(&dev->sem);
    }

    return 0;
}

ssize_t share_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos){
    struct share_dev *dev = filp->private_data;
    ssize_t rst = 0;    //成功读取的字节数

    if (down_interruptible(&dev->sem)){ //申请进入临界区
        pr_info("waiting...\n");
        return -ERESTARTSYS;
    }

    if (*f_pos >= dev->size){   //到达文件尾
        goto out;
    }

    if (*f_pos + count > dev->size){    //过长则截断
        count = dev->size - *f_pos;
    }


    if (!dev->data){    //无数据可读
        pr_info("no data!\n");
        goto out;
    }


    if (copy_to_user(buf, dev->data + *f_pos, (unsigned long)count)){   //数据拷贝到用户空间失败
        pr_info("can't copy data to user\n");
        retval = -EFAULT;
        goto out;
    }


    //更新文件信息
    *f_pos += count;
    rst = count;

    out:
    up(&dev->sem);
    return rst;
}


ssize_t share_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos){
    struct share_dev *dev = filp->private_data;
    share_trim(dev);    //先清空原有的数据
    ssize_t rst = -ENOMEM;  //成功写入的字节数

    if (down_interruptible(&dev->sem)){ //申请进入临界区
        pr_info("waiting...\n");
        return -ERESTARTSYS;
    }


    if (!dev->data){    //数据未分配内存
        dev->data = kmalloc(SHARE_BUFFER_SIZE, GFP_KERNEL);

        if (!dev->data){    //无法分配内存
            pr_info("can't alloc enough memory.\n");
            goto out;
        }

        //初始化内存
        memset(dev->data, 0, SHARE_BUFFER_SIZE);
    }


    if (count > SHARE_BUFFER_SIZE - dev->size){ //过长则截断
        count = SHARE_BUFFER_SIZE - dev->size;
    }


    if (copy_from_user(dev->data + dev->size, buf, count)){ //无法从用户空间拷贝数据
        pr_info("fail to copy from user.\n");
        rst = -EFAULT;
        goto out;
    }


    dev->size += count;
    rst = count;

    out:
    up(&dev->sem);
    return rst;
}

//项目暂时用不上此函数
int share_release(struct inode *inode, struct file *filp)
{
    return 0;
}
//项目暂时用不上这个函数
loff_t share_llseek(struct file *filp, loff_t off, int whence){
    return 0;
}

struct file_operations share_fops = {
        .owner = THIS_MODULE,
        .llseek = share_llseek,
        .read = share_read,
        .write = share_write,
        .open = share_open,
        .release = share_release,
};

void __exit share_exit(void){
    dev_t devno = MKDEV(share_major, share_minor);

    if (share_device){  //清空设备占用的内存
        share_cleanmem(share_device);
        cdev_del(&share_device->cdev);
        kfree(share_device);
    }

    unregister_chrdev_region(devno, 1);
}

//初始化cdev设备
static void share_init_cdev(struct share_dev *dev)
{
    int devno = MKDEV(share_major, share_minor);
    int err;

    cdev_init(&dev->cdev, &share_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &share_fops;
    err = cdev_add(&dev->cdev, devno, 1);
    if(err){
        pr_info("can't add char device %d %d\n", share_major, share_minor);
    }

}

static int __init share_init(void)
{
    int rst;
    dev_t dev = 0;

    if (share_major){   //静态注册
        dev = MKDEV(share_major, share_minor);
        rst = register_chrdev_region(dev, 1, "share");
    }   else{   //动态注册
        rst = alloc_chrdev_region(&dev, share_minor, 1, "share");
        share_major = MAJOR(dev);   //更新主设备号
    }

    if (rst < 0){   //注册失败
        pr_info("can't register char device %d %d\n", share_major, share_minor);
        return rst;
    }

    share_device = kmalloc(sizeof(struct share_dev), GFP_KERNEL);   //给数据结构分配空间
    if (!share_device){ //无法分配空间
        rst = -ENOMEM;
        goto fail;

    memset(share_device, 0, sizeof(struct share_dev));  //初始化数据结构
    sema_init(&share_device->sem, 1);   //初始化互斥信号量
    share_init_cdev(share_device);  //初始化字符设备

    pr_info("register device %d %d\n", share_major, share_minor);
    return 0;

    fail:
    share_exit();
    return rst;
}

module_init(share_init);
module_exit(share_exit);

MODULE_LICENSE("GPL");