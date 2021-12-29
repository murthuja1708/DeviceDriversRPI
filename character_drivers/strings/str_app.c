#include<stdio.h>
#include<errno.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>

#define DATA_SIZE 10
#define BLOCK_SIZE 100

int main(int argc,char* argv[])
{

    char data_read[20];
    //char* data_read=malloc(sizeof(char)*4096);

    FILE* fptr=fopen("file.txt","r");
    int fd=open("/dev/Str",O_RDWR);

   
    if(fd < 0)
    {
        perror("error:");
        return -1;
    }
    /*if(!fptr)
    {
        perror("error:");
        return -1;
    }*/
    else{
        //lseek(fd,0,SEEK_CUR);
        int cnt=read(fd,(char*)data_read,56);
        
        printf("%s\n",data_read);
        
    /*    size_t total_read,total_written=0,nread,nwrite=0;
        size_t len=strlen(block);

        do
        {
            nwrite=write(fd,block+total_written,len);
            total_written+=nwrite;
            //printf("total written bytes %ld and wirtten now %ld\n",total_written,nwrite);
            nread=read(fd,data_read,len);
            total_read+=nread;
            printf("%s\n",data_read);
        } while (total_written < len);*/

/*
        do
        {
            nread=read(fd,data_read,len);
            total_read+=nread;
            printf("total read %ld and data read is %s\n",total_read,data_read);
        } while (total_read < len);

*/
        
        
        

        /*while((nread=fread(block,1,BLOCK_SIZE,fptr)) >0)
        {
            total_read=0;
            block[nread]='\0';
            while (total_read != nread)
            {
                nwrite=write(fd,block,nread);
                printf("bytes written %ld",nwrite);
                nread=read(fd,data_read,nread);
                printf("bytes read %ld\n",nread);
                printf("%s\n",data_read);
                total_read+=nread;
            }
            
        }*/
        close(fd);
        fclose(fptr);
        
    }
    return 0;
}


