#include<stdio.h>
#include<errno.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<pthread.h>

#define BLOCK_SIZE 128


int main(int argc,char* argv[])
{
    char data_read[BLOCK_SIZE];
    
    int fd=open("/dev/ModernFiles",O_RDONLY);

   
    if(fd < 0)
    {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    else{
        
        
        size_t total_read,nread;
        size_t count;

        
        while (nread=read(fd,data_read,BLOCK_SIZE) > 0)
        {
            printf("read %d bytes\n",nread);
            printf("%s\n",data_read);
            printf("\n========\n");
            memset(data_read,0,BLOCK_SIZE);
        }
        
        
        close(fd);
        
    }

    return 0;
}


