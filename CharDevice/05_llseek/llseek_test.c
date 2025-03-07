#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int fd;
    unsigned int ret = 0;
    fd = open("/dev/mtest", O_RDWR);
    if(fd < 0) {
        perror("open node failure!\n");
        return fd;
    }

    write(fd, "hello world ", 13);
    ret = lseek(fd, 0, SEEK_CUR);
    print("ret = %d\n", ret);

    ret = lseek(fd, 0, SEEK_SET);
    print("ret = %d\n", ret);

    char buf[13] = {0};
    read(fd, buf,sizeof(buf));
    print("ret = %s\n", buf);

    ret = lseek(fd, 0, SEEK_CUR);
    print("ret = %d\n", ret);

    ret = lseek(fd, -1, SEEK_CUR);
    print("ret = %d\n", ret);

    ret = lseek(fd, 0, SEEK_SET);
    print("ret = %d\n", ret);

    write(fd, "Linux", 6);
    ret = lseek(fd, 0, SEEK_CUR);
    print("ret = %d\n", ret);

    ret = lseek(fd, 0, SEEK_SET);
    print("ret = %d\n", ret);

    read(fd, buf,sizeof(buf));
    print("ret = %s\n", buf);

    close(fd);
    return 0;
}