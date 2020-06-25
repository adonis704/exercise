#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char* argv[])    //执行的时候输入./main /dev/led_dev 10说明打开的是/dev/led_dev设备，10用于后面的延时10s
{
    char buf[20];
    char test[11] = "hello,world";
    printf("opening...\n");
    int fd = open (argv[1], O_RDWR);
    printf("writing...\n");
    write(fd, test, 20);
    printf("reading\n");
    read (fd, buf, sizeof(buf));
    printf("the result is \n");
    printf("%s", buf);

    close(fd);
    return 0;
}