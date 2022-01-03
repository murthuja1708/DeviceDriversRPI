#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include "timer_driver.h"






int main()
{
    int fd;

    fd=open("/dev/timer_driver",O_RDWR);
    int status;
    if(fd<0)
    {
        perror("error");
        exit(EXIT_FAILURE);
    }
    printf("enter the amount of time in milliseconds to toggle led\n");
    int delay_time;
    scanf("%d",&delay_time);
    printf("delay time is %d\n",delay_time);
    ioctl(fd,set_periodicity,&delay_time);


    close(fd);

    return 0;
}