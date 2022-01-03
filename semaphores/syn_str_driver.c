#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fcntl.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/semaphore.h>

#define MAX_BLOCK_SIZE 512

char data[MAX_BLOCK_SIZE]={'\0'};



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");
MODULE_DESCRIPTION("semaphores drivers\n");


static int calc_open(struct inode *inode, struct file *file);

static int  calc_release(struct inode *inode, struct file *file);


static ssize_t Write_Str(struct file *,const char *, size_t, loff_t *);

static ssize_t Read_Str(struct file *, char*, size_t, loff_t *);


dev_t calc_dev=0;
struct cdev kcalc_dev;

struct semaphore data_sema;



const char *device_path="/dev/ModernFiles";


static struct file_operations fops={
    .owner      = THIS_MODULE,
    .read     = Read_Str,
    .write    = Write_Str,
    .open     = calc_open,
    .release  = calc_release,
};



static int __init Str_device_initialization(void)
{
    calc_dev = MKDEV(405,1238);
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

static int calc_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"Kernel Open Call\n");
    sema_init(&data_sema,1);
    return 0;
}

static ssize_t Write_Str(struct file *filp, const char *buf, size_t len, loff_t * off)
{
    ssize_t retvalue=-1;
    
    unsigned long result=0;

    printk(KERN_INFO "reading from offset %lld checking and truncating the len",*off);

    if(*off + len > MAX_BLOCK_SIZE)
    {
        len=MAX_BLOCK_SIZE - *off;
    }
    down(&data_sema);
    //Returns number of bytes that could not be copied.
    result=copy_from_user((char*)data,(char*)buf,len);

    if(result == 0)
    {
        printk(KERN_INFO"Data Successfully Written \n");
        retvalue=len;
        return retvalue;
    }
    else if(result > 0)
    {
        retvalue=len-result;
        return retvalue;
    }
    printk(KERN_ALERT"Error writing Data\n");
    return retvalue;

}


static ssize_t Read_Str(struct file *filp, char* buf, size_t len, loff_t * off)
{
    ssize_t retvalue=-1;
    unsigned long str_len,result;

    if(*off + len > MAX_BLOCK_SIZE)
    {
        len=MAX_BLOCK_SIZE - *off;
    }

    printk(KERN_INFO "reading %ld bytes\n",len);

    char *temp=data;
    size_t i=0;
    while (*temp!='\0' && i<MAX_BLOCK_SIZE)
    {
        if(*temp >= 'a' && *temp<='z')
        {
            *temp=*temp-32;
        }
        temp++;
        i++;
    }
    

    /*Returns number of bytes that could not be copied.*/
    result=copy_to_user(buf,data,len);
    up(&data_sema);

    if(result>=0)
    {
        retvalue=len-result;
        return  retvalue;
    }
    printk(KERN_ALERT "\n ERROR WRITING DATA TO USER\n ");
    return retvalue;
    

}


static int  calc_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"release call\n");
    return 0;
}


static void __exit Str_device_release(void)
{
    
    unregister_chrdev_region(calc_dev,1);
    printk(KERN_INFO "unregistered character device successfully\n");
    cdev_del(&kcalc_dev);
    printk(KERN_INFO "deleted  character device from kernel\n");

}

module_init(Str_device_initialization);
module_exit(Str_device_release);
