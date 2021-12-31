#include<stdio.h>
#include<errno.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<pthread.h>

#define BLOCK_SIZE 64


int main(int argc,char* argv[])
{

    char block[BLOCK_SIZE];
    char data_read[BLOCK_SIZE];
    

    FILE* fptr=fopen("file2.txt","r");
    
    if(!fptr)
    {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    int fd=open("/dev/ModernFiles",O_RDWR);

   
    if(fd < 0)
    {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    else{
        
        
        size_t total_read,total_written=0,nread,nwrite=0;
        size_t count;
        
        while((count=fread(block,1,BLOCK_SIZE-1,fptr)) > 0)
        {
            block[count]='\0';
            total_written=0;
            printf("count is %ld\n",count);
            do
            {
                nwrite=write(fd,block+total_written,count);
                total_written+=nwrite;
                nread=read(fd,data_read,nwrite);
                total_read+=nread;
                printf("\n=====thread1===\n");
                printf("%s\n",data_read);
                memset(data_read,0,BLOCK_SIZE);
            } while (total_written < count);
        }
        
        close(fd);
        fclose(fptr);
        
    }

    return 0;
}


