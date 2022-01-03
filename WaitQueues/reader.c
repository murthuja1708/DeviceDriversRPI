#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<pthread.h>

int fd;


int main(int argc,char* argv[])
{
    fd=open("/dev/WaitQueues",O_RDWR);
    if(fd<0)
    {
        perror("error:");
        exit(EXIT_FAILURE);

    }
    int data;

    read(fd,&data,4);
    printf("data:%d\n",data);
    close(fd);
    return 0;
}