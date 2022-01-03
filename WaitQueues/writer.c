#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<pthread.h>

int fd;


/*void* process_data()
{
    int return_data=0;
    printf("getting data\n");
    while (return_data!=0)
    {
        read(fd,&return_data,4);
        printf("data read %d\n",return_data);
    }
}*/


int main(int argc,char* argv[])
{
    fd=open("/dev/WaitQueues",O_RDWR);
    if(fd<0)
    {
        perror("error:");
        exit(EXIT_FAILURE);

    }
    int data;

    printf("enter a number\n");
    scanf("%d",&data);

    write(fd,(int*)&data,4);
    
    close(fd);
    return 0;
}