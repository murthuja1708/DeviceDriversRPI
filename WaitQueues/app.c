#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<pthread.h>

int fd;

void* get_user_data()
{
    printf("enter num\n");
    int* num=NULL;
    scanf("%d",num);
    return num;
}


void* process_data()
{
    int return_data=0;
    printf("getting data\n");
    while (return_data!=0)
    {
        read(fd,&return_data,4);
        printf("data read %d\n",return_data);
    }
}


int main(int argc,char* argv[])
{
    fd=open("/dev/WaitQueues",O_RDWR);
    if(fd<0)
    {
        perror("error:");
        exit(EXIT_FAILURE);

    }
    pthread_t reader_id;
    pthread_t writer_id;

    
    
    pthread_create(&reader_id,NULL,get_user_data,NULL);
    pthread_create(&writer_id,NULL,process_data,NULL);

    void *data;

    pthread_join(reader_id,&data);
    write(fd,(int*)&data,4);

    pthread_join(writer_id,NULL);
    
    close(fd);
    return 0;
}