#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fcntl.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/wait.h>
#include<linux/jiffies.h>
#include<linux/semaphore.h>

#define TIME_OUT 10000

int k_data=0;

struct semaphore k_data_sema;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");
MODULE_DESCRIPTION("driver that creates and calculates add,sub,div and mul\n");

static int calc_open(struct inode *inode, struct file *file);

static int  calc_release(struct inode *inode, struct file *file);


static ssize_t set_data(struct file *,const char *, size_t, loff_t *);

static ssize_t get_data(struct file *, char*, size_t, loff_t *);

const char *device_path="/dev/WaitQueues";


//wait_queue_head_t driver_queue;

static int flag=0;

dev_t calc_dev=0;
struct cdev kcalc_dev;

DECLARE_WAIT_QUEUE_HEAD(driver_queue);

static struct file_operations fops={
    .owner      = THIS_MODULE,
    .read     = get_data,
    .write    = set_data,
    .open     = calc_open,
    .release  = calc_release,
};




static int __init WaitQueue_device_initialization(void)
{
    calc_dev = MKDEV(410,1210);
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
    
    return 0;
}

static ssize_t set_data(struct file *filp, const char *buf, size_t len, loff_t * off)
{
    unsigned long result=copy_from_user(&k_data,(int*)buf,4);
    if(result == 0)
    {
        flag=1;
        printk(KERN_DEBUG " awakening the readers...\n");
        wake_up_interruptible(&driver_queue);
        return len;
    }
    else if(result>0)
    {
        return len-result;
    }
   
    return len;

}


static ssize_t get_data(struct file *filp, char* buf, size_t len, loff_t * off)
{
    unsigned long result;
    int wait_res;
    
    while (flag==0)
    {
        printk(KERN_DEBUG "process going to sleep\n");
        wait_res=wait_event_interruptible(driver_queue,flag!=0);
        printk(KERN_DEBUG "awoken\n");
    }

    k_data*=2;
    printk(KERN_INFO"COPYING DATA TO USER\n");
    result=copy_to_user((int*)buf,&k_data,4);
    
    if(result == 0)
    {
        flag=0;
        return len;
    }
    else if(result>0)
    {
        return len-result;
    }
    
    return 0;

}

static int  calc_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"release call\n");
    return 0;
}


static void __exit WaitQueue_device_release(void)
{
    
    unregister_chrdev_region(calc_dev,1);
    printk(KERN_INFO "unregistered character device successfully\n");
    cdev_del(&kcalc_dev);
    printk(KERN_INFO "deleted  character device from kernel\n");

}

module_init(WaitQueue_device_initialization);
module_exit(WaitQueue_device_release);
