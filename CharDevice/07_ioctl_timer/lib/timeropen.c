#include "timerlib.h"


int timer_open(int fd) {
    int ret;
    ret = ioctl(fd, TIMER_OPEN);
    if(ret) {
        printf("ioctl open error\n");
        exit(-1);
    }
    return ret;
}