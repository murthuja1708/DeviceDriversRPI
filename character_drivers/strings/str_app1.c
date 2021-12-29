#include<stdio.h>
#include<errno.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>


#define BLOCK_SIZE 4096

int main(int argc,char* argv[])
{

    char block[BLOCK_SIZE];
    char data_read[BLOCK_SIZE];
    

    FILE* fptr=fopen("file.txt","r");
    
    if(!fptr)
    {
        perror("error:");
        return -1;
    }

    int fd=open("/dev/Str",O_RDWR);

   
    if(fd < 0)
    {
        perror("error:");
        return -1;
    }
    else{
        
        
        size_t total_read,total_written=0,nread,nwrite=0;
        size_t count;
        
        while((count=fread(block,1,BLOCK_SIZE,fptr)) > 0)
        {
            total_written=0;
            printf("count is %ld\n",count);
            do
            {
                nwrite=write(fd,block+total_written,count);
                printf("written  %ld\n",nwrite);
                total_written+=nwrite;
                nread=read(fd,data_read,nwrite);
                total_read+=nread;
                printf("read  %ld\n",nread);
                printf("%s\n",data_read);
                memset(data_read,0,BLOCK_SIZE);
            } while (total_written < count);
        }
        
        close(fd);
        fclose(fptr);
        
    }
    return 0;
}


