#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include "ioctl_driver.h"


enum baudrate{
    serial_baudrate_1200=1200,
    serial_baudrate_2400=2400,
    serial_baudrate_4800=4800,
    serial_baudrate_9600=9600,
    serial_baudrate_19200=19200,
    serial_baudrate_38400=38400,
    serial_baudrate_57600=57600,
    serial_baudrate_115200=115200
    }baud;

enum DataFram{
serial_data_frame_7=7,
serial_data_frame_8=8,
serial_data_frame_9=9
}frame;

enum Parity{
    serial_parity_none='N',
    serial_parity_even='E',
    serial_parity_odd='O'
};

enum StopBits{
    serial_stop_1=0,
    serial_stop_1=1
};


int main()
{
    int fd;

    fd=open("/dev/ioctl_driver",O_RDWR);
    int status;
    if(fd<0)
    {
        perror("error");
        exit(EXIT_FAILURE);
    }
    int baud=serial_baudrate_115200;
    int frame=serial_data_frame_8;
    int _parity=serial_parity_even;

    ioctl(fd,serial_baud_set,&baud);
    ioctl(fd,serial_databits_set,&frame);
    ioctl(fd,serial_parity_set,&_parity);

    int all=serial_baudrate_115200|serial_data_frame_8|serial_parity_even;
    ioctl(fd,serial_baud_set|serial_databits_set|serial_parity_set,all);


    close(fd);

    return 0;
}