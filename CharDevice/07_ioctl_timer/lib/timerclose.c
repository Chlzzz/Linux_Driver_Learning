#include "timerlib.h"


int timer_close(int fd) {
    int ret;
    ret = ioctl(fd, TIMER_CLOSE);
    if(ret) {
        printf("ioctl close error\n");
        exit(-1);
    }
    return ret;
}