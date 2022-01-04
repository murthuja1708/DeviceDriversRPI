#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>


int main(int argc,char* argv[])
{
    int fd[4];
    int operands[2];
    char* devices[4]={
        "/dev/Add",
        "/dev/Sub",
        "/dev/Mul",
        "/dev/Div"
    };
    printf("enter two number\n");
    
    scanf("%d,%d",&operands[0],&operands[1]);

    printf("entered numbers are %d and %d\n",operands[0],operands[1]);
    
    for(size_t i=0;i<4;i++)
    {
        fd[i]=open(devices[i],O_RDWR);
        if(fd[i] == -1)
        {
            perror("error while opening device\n");
            return -1;
        }
        

        write(fd[i],operands,8);
        
        int res=0;

        printf("reading data:");

        read(fd[i],&res,4);

        printf("%d\n",res);
        
        close(fd[i]);
    }
    
    
    return 0;
}