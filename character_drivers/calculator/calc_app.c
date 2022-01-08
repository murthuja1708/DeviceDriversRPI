#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>


int main(int argc,char* argv[])
{
    int fd;
    int operands[2];
    int nwrite,nread;
    char* devices[4]={
        "/dev/Add",
        "/dev/Sub",
        "/dev/Mul",
        "/dev/Div"
    };

    int choice;
    printf("enter 0-->/dev/Add\n1-->/dev/Sub\n2-->/dev/Mul\n3-->/dev/Div\n");
    scanf("%d",&choice);
    if(choice>3)
    {
        return -1;
    }

    /* RDWR-->31 &&mode-->917535
       O_RDONLY-->29 && mode-->655389
       O_WRONLY-->30 && mode-->786462

    */
    //fd=open(devices[choice],O_WRONLY);
    fd=open(devices[choice],O_RDONLY);
    //fd=open(devices[choice],O_WRONLY);
    
    printf("enter two number\n");
    
    scanf("%d,%d",&operands[0],&operands[1]);

    printf("entered numbers are %d and %d\n",operands[0],operands[1]);
    
    //for(size_t i=0;i<4;i++)
    //{
        
        if(fd == -1)
        {
            perror("error while opening device\n");
            return -1;
        }
        
        nwrite=write(fd,operands,8);
        if(nwrite<0)
        {
            printf("wrote %d bytes\n",nwrite);
            perror("error");
            //return -1;
        }
        
        
        int res=0;

        printf("reading data:");
        nread=read(fd,&res,4);
        if(nread<0)
        {
            printf("read %d bytes\n",nread);
            perror("error");
            //return -1;
        }

        printf("%d\n",res);
        
        close(fd);
    //}
    
    
    return 0;
}