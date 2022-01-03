#ifndef IOCTL_DRIVER_H
#define IOCTL_DRIVER_H

#define serial_magic '{'


//setting up ioctl command numbers

#define serial_reset _IO(serial_magic,0)
#define serial_baud_set _IOW(serial_magic,1,int)
#define serial_stopbits_set _IOW(serial_magic,2,int)
#define serial_databits_set _IOW(serial_magic,3,int)
#define serial_parity_set _IOW(serial_magic,4,int)
#define serial_all _IOW(serial_magic,5,int)
#define serial_status _IOR(serial_magic,6,int)
//#define serial_control _IOWR(serial_magic,6,char)

#define serial_max_nr 8

struct uart{
    unsigned int baud_rate;
    unsigned int stop_bits;
    unsigned char parity;
};




#define serial_maxnr 7



#endif