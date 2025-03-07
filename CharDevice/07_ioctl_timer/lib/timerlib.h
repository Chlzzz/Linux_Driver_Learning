#ifndef _TIMERLIB_H
#define _TIMERLIB_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>


#define TIMER_OPEN _IO('L', 0)     
#define TIMER_CLOSE _IO('L', 1)
#define TIMER_SET _IOW('L', 2, int)


int timer_open(int fd);
int timer_close(int fd);
int timer_set(int fd, int arg);


#endif