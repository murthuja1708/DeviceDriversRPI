#include<stdio.h>
#include<errno.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>





int main(int argc,char* argv[])
{
    static int block_size=0;
    FILE* fptr;
    size_t total_read=0,total_written=0;
    size_t count=0;
    ssize_t nread=0,nwrite=0;
    char block[4096];
    size_t arr_index=0;
    pid_t fid=fork();
    if(fid<0)
    {
        perror("error in creating child process:");
        exit(EXIT_FAILURE);
    }
    
    else if(fid==0)
    {
        total_read=0;
        total_written=0;
        block_size=64;
        fptr=fopen("file1.txt","r");

        if(!fptr)
        {
            perror("error while opening file1:");
            exit(EXIT_FAILURE);
        }
    }
    else{
            total_read=0;
            total_written=0;
            block_size=128;
            fptr=fopen("file2.txt","r");

            if(!fptr)
            {
                perror("error while opening file1:");
                exit(EXIT_FAILURE);
            }
        
    }
    

    int fd=open("/dev/ModernFiles",O_RDWR);
    //int fd=open("/home/Assignments/ldd/semaphores/file3.txt",O_RDWR);


    if(fd < 0)
    {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    else{
        
        while ((count=fread(&block[total_read],1,block_size-1,fptr))>0)
        {
            /* code */
            total_written=0;
            block[count]='\0';
            printf("count is %d by %d\n",count,getpid());
            do
            {
                nwrite=write(fd,&block[arr_index],count-total_written);
                printf("wrote %d bytes %d from index %d\n",nwrite,getpid(),arr_index);
                total_written+=nwrite;
                arr_index+=nwrite;   
                printf("total_written-->%d && count-->%d\n",total_written,count);
            } while (nwrite > 0 && total_written<count);
            
            total_read+=count;
        }

        if(count<0 || nwrite <0)
        {
            perror("error:");
            exit(EXIT_FAILURE);
        }
        
        close(fd);
        fclose(fptr);
        
    }
    return 0;
}


