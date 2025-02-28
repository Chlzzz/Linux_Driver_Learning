#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


// 设备类型，序列号，方向， 数据尺寸
// type     nr          size
// 定义命令
// #define CMD_TEST0 _IO('L', 0)       // 无数据的命令

// #define CMD_TEST1 _IO('A', 0)
#define CMD_TEST2 _IOW('A', 1, int)
// #define CMD_TEST3 _IOR('A', 2, int)
// #define CMD_TEST4 _IOWR('A', 3, int)

struct args {
    int a;
    char b;
    char c[10];
};

int main(int argc, char* argv[]) {
   
    // 分解， 从定义好的命令中分解出需要的方向/尺寸... 
    // print("CMD_TEST3 type is : %ld\n", _IOC_TYPE(CMD_TEST3));
    // print("CMD_TEST3 dir is : %ld\n", _IOC_DIR(CMD_TEST3));
    // print("CMD_TEST3 nr is : %ld\n", _IOC_NR(CMD_TEST3));
    // print("CMD_TEST3 size is : %ld\n", _IOC_SIZE(CMD_TEST3));

    int fd;
    unsigned int ret = 0;
    struct args test;
    test.a = 1;
    test.b = 'i';
    strcpy(test.c, "5v");

    
    fd = open("/dev/mtest", O_RDWR);
    if(fd < 0) {
        perror("open node failure!\n");
        return fd;
    }

    while(1) {
        // 应用层ioctl调用驱动的ioctl
        //ioctl(fd, CMD_TEST0);
        ioctl(fd, CMD_TEST2, &test);
        sleep(1);
    }
}