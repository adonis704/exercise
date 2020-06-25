#ifndef _MYDRIVER_H
#define _MYDRIVER_H

#define SHARE_MAJOR 0
#define SHARE_BUFFER_SIZE PAGE_SIZE //将大小限制为页大小

struct share_dev {
    char *data;
    unsigned long size;
    struct semaphore sem;
    struct cdev cdev;
};

#endif