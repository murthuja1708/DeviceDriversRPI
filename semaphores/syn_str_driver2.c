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
#include<linux/completion.h>

#define MAX_BLOCK_SIZE 512

DECLARE_COMPLETION(data_read_completion);
DECLARE_COMPLETION(data_write_completion);

DEFINE_SEMAPHORE(data_sema);

char data[MAX_BLOCK_SIZE]={'\0'};
size_t *off=0;

dev_t calc_dev=0;
struct cdev kcalc_dev;


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");
MODULE_DESCRIPTION("semaphores drivers\n");



static int Sema_open(struct inode *inode, struct file *file);

static int  Sema_release(struct inode *inode, struct file *file);

static ssize_t Write_Str(struct file *,const char __user*, size_t, loff_t *);

static ssize_t Read_Str(struct file *, char __user*, size_t, loff_t *);


struct semaphore data_sema;


//DECLARE_WAIT_QUEUE_HEAD(driver_queue);

const char *device_path="/dev/ModernFiles";


static struct file_operations fops={
    .owner      = THIS_MODULE,
    .open     = Sema_open,
    .release  = Sema_release,
    .write=Write_Str,
    .read=Read_Str,
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


static int Sema_open(struct inode *inode, struct file *filp)
{
    
    
    //int perm;
    printk(KERN_INFO"Kernel Open Call\n");
    /*perm=(filp->f_mode & 0x3);
    printk(KERN_INFO"f_mode is %d and perm is %d\n",filp->f_mode,perm);
    switch (perm)
    {
    case 1:
        printk(KERN_INFO"read\n");
        fops.read=Read_Str;
        break;
    case 2:
        printk(KERN_INFO"write\n");
        fops.write=Write_Str;
        break;
    case 3:
        printk(KERN_INFO"read and write\n");
        fops.write=Write_Str;
        fops.read=Read_Str;
        break;
    }*/
    
    return 0;
}



static ssize_t Write_Str(struct file *filp, const char *buf, size_t len, loff_t * off)
{
    ssize_t retvalue;
    unsigned long result;

    int mode;
    mode=filp->f_mode;
    printk(KERN_INFO"in WRITE mode is %d\n",mode);
    mode&=0x3;
    printk(KERN_INFO"in WRITE after masking mode is %d\n",mode);

    if(mode ==1)   
    {
        printk("error\n");
        return -1;
    }
    
    printk(KERN_INFO"file mode %d\n",filp->f_mode);
    

    if(*off+len > MAX_BLOCK_SIZE)
    {
        len=MAX_BLOCK_SIZE-*off;
    }

    printk(KERN_INFO"acquiring  sema\n");

    
    //Returns number of bytes that could not be copied.
    printk("writing %d bytes from pos %lld",len,*off);
    down_killable(&data_sema);
    result=copy_from_user(&data[*off],(char*)buf,len);
    if(result == 0)
    {
        printk(KERN_INFO"Data Successfully Written \n%s\n",data);
        retvalue=len;
        *off+=len;
        
    }
    else if(result > 0)
    {
        retvalue=len-result;
        *off+=retvalue;
        
    }

    
    complete(&data_write_completion);
    printk(KERN_INFO"signalled  that write  is done\n");
    
    printk(KERN_INFO"waiting for data to be read\n");
    wait_for_completion_killable(&data_read_completion);
    printk(KERN_INFO"waiting done\n");
    
    printk(KERN_INFO"releasing sema\n");
    up(&data_sema);
    printk(KERN_INFO"released sema\n");
    
    
    
    //printk(KERN_ALERT"Error writing Data\n");
    return retvalue;
}


static ssize_t Read_Str(struct file *filp, char* buf, size_t len, loff_t * off)
{
    ssize_t retvalue=-1;
    unsigned long result;
    char *temp;
    int i;

    int mode;
    mode=filp->f_mode;
    printk(KERN_INFO"in READ mode is %d\n",mode);
    mode&=0x3;
     if(mode ==2) 
    {
        printk("error\n");
        return -1;
    }


    if(*off+len > MAX_BLOCK_SIZE)
    {
        len=MAX_BLOCK_SIZE-*off;
    }

    printk(KERN_INFO"waiting for data to be written in read\n");

    wait_for_completion_killable(&data_write_completion);
    printk(KERN_INFO"waiting done now we can read\n");
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
    result=copy_to_user(buf,&data[*off],len);
    
    if(result==0)
    {   
        complete(&data_read_completion);
        printk(KERN_INFO"signalled  that read  is done\n");

        return len;
    }
    else if(result > 0)
    {
        retvalue=len-result;
        complete(&data_read_completion);
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
    *off=0;
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
