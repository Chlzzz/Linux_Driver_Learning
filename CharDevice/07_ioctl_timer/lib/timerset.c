#include "timerlib.h"


int timer_set(int fd, int arg) {
    int ret;
    ret = ioctl(fd, TIMER_SET, arg);
    if(ret) {
        printf("ioctl set error\n");
        exit(-1);
    }
    return ret;
}