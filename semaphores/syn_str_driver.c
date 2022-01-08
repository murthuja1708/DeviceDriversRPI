#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fcntl.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>



#define MAX_BLOCK_SIZE 512

char data[MAX_BLOCK_SIZE]={'\0'};
static int flag=0;
size_t arr_index=0;

dev_t calc_dev=0;
struct cdev kcalc_dev;


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");
MODULE_DESCRIPTION("semaphores drivers\n");



static int Sema_open(struct inode *inode, struct file *file);

static int  Sema_release(struct inode *inode, struct file *file);

static ssize_t Write_Str(struct file *,const char __user*, size_t, loff_t *);

static ssize_t Read_Str(struct file *, char __user*, size_t, loff_t *);




//DEFINE_SEMAPHORE(data_sema);



//DECLARE_WAIT_QUEUE_HEAD(driver_queue);

const char *device_path="ModernFiles";


static struct file_operations fops={
    .owner      = THIS_MODULE,
    .read     = Read_Str,
    .write    = Write_Str,
    .open     = Sema_open,
    .release  = Sema_release,
};



static int __init Str_device_initialization(void)
{
    calc_dev = MKDEV(405,0);
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

static int Sema_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"Kernel Open Call\n");
    
    
    return 0;
}

static ssize_t Write_Str(struct file *filp, const char __user *buf, size_t len, loff_t * off)
{
    ssize_t retvalue;
    unsigned long result;
    
    printk("writing %d bytes from pos %ll
    d",len,filp->f_pos);
    
    if(arr_index+len > MAX_BLOCK_SIZE)
    {
        len=MAX_BLOCK_SIZE-arr_index;
    }

    printk(KERN_INFO"acquiring  sema\n");

    //Returns number of bytes that could not be copied.
    
    result=copy_from_user(&data[arr_index],(char*)buf,len);
    if(result == 0)
    {
        printk(KERN_INFO"Data Successfully Written \n%s\n",data);
        retvalue=len;
        flag=1;
        arr_index+=len;
        
        //wake_up_interruptible(&driver_queue);
        
    }
    else if(result > 0)
    {
        retvalue=len-result;
        arr_index+=retvalue;
        
    }
    
    //printk(KERN_ALERT"Error writing Data\n");
    return retvalue;
}


static ssize_t Read_Str(struct file *filp, char* buf, size_t len, loff_t * off)
{
    ssize_t retvalue=-1;
    unsigned long result;
    char *temp;
    int i;


    printk("reading %d bytes from pos %d",len,arr_index);

    if(arr_index+len > MAX_BLOCK_SIZE)
    {
        len=MAX_BLOCK_SIZE-arr_index;
    }
    


    printk(KERN_INFO"reading %d bytes from pos %d\n",len,arr_index);

    temp=data;
    i=0;
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
    result=copy_to_user(buf,data+arr_index,len);
    
    if(result==0)
    {
        printk(KERN_INFO"released sema\n");
        flag=0;
        arr_index+=len;

        return len;
    }
    else if(result > 0)
    {
        retvalue=len-result;
        arr_index+=len;
        return retvalue;
    }


    else{
        printk(KERN_ALERT "\n ERROR WRITING DATA TO USER\n ");
    }
    return 0;
    

}


static int  Sema_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"release call\n");
    data[0]='\0';
    arr_index=0;
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
