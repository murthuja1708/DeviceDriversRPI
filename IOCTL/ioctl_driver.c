#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/ioctl.h>
#include<linux/fs.h>  //struct file
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include "ioctl_driver.h"



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");
MODULE_DESCRIPTION("example for creating ioctl driver\n");

int baudrate;

static int ioctl_open(struct inode* inode,struct file* filep);
static int ioctl_close(struct inode* inode,struct file* filep);
static long int my_ioctl(struct file *, unsigned int, unsigned long);


static struct file_operations fops={
    .owner      = THIS_MODULE,
    .open     = ioctl_open,
    .unlocked_ioctl  =my_ioctl,
    .release  = ioctl_close,
};

const char *device_path="/dev/ioctl_driver";

dev_t calc_dev=0;
struct cdev kcalc_dev;



static int ioctl_open(struct inode* inode,struct file* filep)
{
    printk(KERN_INFO"Open call\n");
    return 0;
}

static long int my_ioctl(struct file* filep,unsigned cmd,unsigned long argp)
{
     long result;
     struct uart uart_params;
     
    switch (cmd)
    {
    case serial_baud_set:
        printk(KERN_INFO"command we got %i\n",cmd);
        result=copy_from_user(&baudrate,(int*)argp,sizeof(baudrate));
        printk("result %ld",result);
        if(result==0)
        {
            printk(KERN_INFO"value we got %i\n",baudrate);

        }
        if(result>0)
        {
            return result;
        }
        break;
    case serial_stopbits_set:
        printk(KERN_ALERT"serial stop bits set\n");
        break;

    case serial_parity_set:
        printk(KERN_ALERT"serial parity bit set\n");
        break;

    case serial_status:
        printk(KERN_ALERT"serial status read\n");
        break;
    
    case serial_baud_set|serial_stopbits_set|serial_parity_set:
        printk(KERN_ALERT"serial all\n");
        result=copy_from_user((struct uart*)&uart_params,(struct uart*)argp,sizeof(struct uart));
        printk(KERN_INFO"BAUD-RATE %d",uart_params.baud_rate);
        printk(KERN_INFO"stop bits %i\n",uart_params.stop_bits);
        printk(KERN_INFO"PARITY %c\n",uart_params.parity);
        break;
    default:
        printk(KERN_ALERT"default\n");
        break;
    }
    return result;
}

static int ioctl_close(struct inode* inode,struct file* filep)
{
    printk(KERN_INFO"close call\n");
    return 0;
}

static int __init ioctl_device_initialization(void)
{
    calc_dev = MKDEV(500,1500);
    if(register_chrdev_region(calc_dev, 1,device_path) < 0)
    {
        printk(KERN_ALERT"\ncould not allocate device numbers\n");
        return -1;
    }
    
    cdev_init(&kcalc_dev,&fops);  //kcalc_dev to initialize with related file operations retuns nothing

    if(cdev_add(&kcalc_dev,calc_dev,1) < 0)
    {
        unregister_chrdev_region(calc_dev,1);
        printk(KERN_ALERT "\ncould not add devices to kernel\n");
        return -1;
    }
    
    printk(KERN_INFO "\nadded Str device to kernel\n");

    
    return 0;
}

void ioctl_device_release(void)
{
    unregister_chrdev_region(calc_dev,1);
    printk(KERN_INFO "unregistered character device successfully\n");
    cdev_del(&kcalc_dev);
    printk(KERN_INFO "deleted  character device from kernel\n");
}

module_init(ioctl_device_initialization);
module_exit(ioctl_device_release);