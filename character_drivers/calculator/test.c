#include<stdio.h>

struct some_data{
    int a;
    char b;
    int c;
    int d;
}data;

int main(int argc,char* argv[])
{

    const struct some_data data={
        1234,'a',1256,5678
    };

    struct some_data *pointer_data=&data;

    pointer_data->a=10;
    printf("%d\n%c\n",pointer_data->a,pointer_data->b);


    return 0;
}