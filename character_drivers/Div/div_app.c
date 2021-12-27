#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

int main(int argc,char* argv[])
{
    if(argc>1)
    {
    int fd=open("/dev/Div",O_RDWR);
    if(fd == -1)
    {
        perror("error while opening device\n");
        return -1;
    }
    int operands[2]={0};

    operands[0]=atoi(argv[1]);
    write(fd,&operands[0],1*sizeof(int));

    operands[1]=atoi(argv[2]);

    write(fd,&operands[1],1*sizeof(int));
    
    int div=0;

    printf("reading data:");

    read(fd,&div,1*sizeof(int));

    printf("%d\n",div);

    close(fd);
    }

    /*unlink("/dev/Add");
    printf("%s\n","unlinked  character device file from user space");*/
    return 0;

}
