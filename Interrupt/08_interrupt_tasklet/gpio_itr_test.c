#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int fd = 0;
    unsigned int counter = 0;
    // char buf1[32] = {0};
    // char buf2[32] = "hello!";

    fd = open("/dev/mtest", O_RDWR);
    if(fd < 0) {
        perror("open node failure!\n");
        return fd;
    }

    // write(fd, buf2, sizeof(buf2));
    // if(fd < 0) {
    //     perror("write failure!\n");
    //     return fd;
    // }
    
    while(1) {
        read(fd, &counter, sizeof(counter));
        if(fd < 0) {
            perror("read failure!\n");
            return fd;
        }
        printf("now, buf1 is : %s\n", counter);
        sleep(1);
    }

    close(fd);
    return 0;
}
