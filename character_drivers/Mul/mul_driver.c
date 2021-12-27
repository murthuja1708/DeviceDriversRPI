#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fcntl.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");
MODULE_DESCRIPTION("driver that creates and calculates add,sub,div and mul\n");

#define INT_SIZE 4


static int calc_open(struct inode *inode, struct file *file);

static int  calc_release(struct inode *inode, struct file *file);


static ssize_t Write_Mul(struct file *,const char *, size_t, loff_t *);

static ssize_t Read_Mul(struct file *, char*, size_t, loff_t *);



dev_t calc_dev=0;
struct cdev kcalc_dev;

int operands[2]={0};
static size_t arr_index=0;

const char *device_path="/dev/Mul";


static struct file_operations fops={
    .owner      = THIS_MODULE,
    .read     = Read_Mul,
    .write    = Write_Mul,
    .open     = calc_open,
    .release  = calc_release,
};



static int __init Mul_device_initialization(void)
{
    calc_dev = MKDEV(403,1236);
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

    printk(KERN_INFO "\nadded devices to kernel\n");

    
    return 0;
}

static int calc_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"Kernel Open Call\n");
    arr_index=0;
    return 0;
}

static ssize_t Write_Mul(struct file *filp, const char *buf, size_t len, loff_t * off)
{
    ssize_t retvalue=-1;
    
    int temp;
    unsigned long result=0;

    if(*off + len > INT_SIZE)
    {
        len=INT_SIZE - *off;
    }

    //Returns number of bytes that could not be copied.
    result=copy_from_user(&temp,(char*)buf,len);

    if(result == 0)
    {
        printk(KERN_INFO"Data Successfully Written \n");
        operands[arr_index]=temp;
        arr_index++;
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


static ssize_t Read_Mul(struct file *filp, char* buf, size_t len, loff_t * off)
{
    ssize_t retvalue=-1;
    int _mul=0;
    unsigned long str_len,result;

    if(*off + len > INT_SIZE)
    {
        len=INT_SIZE - *off;
    }
    

    printk(KERN_INFO "Kernel READ Call\n");

    printk(KERN_INFO"1st & 2nd operand in read %d %d\n",operands[0],operands[1]);
    
    _mul=operands[0]*operands[1];
    
    printk(KERN_INFO"copying %d to kernel space from user space",_mul);
    

    /*Returns number of bytes that could not be copied.*/
    result=copy_to_user(buf,&_mul,len);

    
    if(result>=0)
    {
        retvalue=len-result;
        return retvalue;    
    }
    printk(KERN_ALERT "\n ERROR WRITING DATA TO USER\n ");
    return retvalue;
    

}


static int  calc_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"resetting array index in release call\n");
    arr_index=0;
    return 0;
}


static void __exit Mul_device_release(void)
{
    
    unregister_chrdev_region(calc_dev,1);
    printk(KERN_INFO "unregistered character device successfully\n");
    cdev_del(&kcalc_dev);
    printk(KERN_INFO "deleted  character device from kernel\n");

}

module_init(Mul_device_initialization);
module_exit(Mul_device_release);
