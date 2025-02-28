#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "timerlib.h"


// 设备类型，序列号，方向， 数据尺寸
// type     nr          size
// 定义命令
// #define TIMER_OPEN _IO('L', 0)       // 无数据的命令


int main(int argc, char* argv[]) {
   
    // 分解， 从定义好的命令中分解出需要的方向/尺寸... 

    int fd;
    unsigned int ret = 0;
    
    fd = open("/dev/mtest", O_RDWR);
    if(fd < 0) { 
        perror("open node failure!\n");
        return fd;
    }

    timer_set(fd, 1000);
    timer_open(fd);
    sleep(3);
    timer_set(fd, 3000);
    sleep(7);
    timer_close(fd);

    close(fd);
    return 0;

}