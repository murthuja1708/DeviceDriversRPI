
#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fcntl.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>

#include"calc_operations.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");
MODULE_DESCRIPTION("driver that creates and calculates add,sub,div and mul\n");

static size_t arr_index=0;


/*dev_t calc_dev=0;
struct cdev kcalc_dev;*/



const char *device_path="/dev/Calc";

static struct file_operations fops={
            .owner      = THIS_MODULE,
            .open     = calc_open,
            .release  = calc_release,
        };




/*

cdev_ --> dev_t dev && file_operations* ops 
inode  --> dev_t i_rdev  ,file_operations* i_fop,cdev i_cdev

*/



static int __init Calc_device_initialization(void)
{
    int ret;
    size_t i,j;
    drv_prvdata.device_number = MKDEV(451,1251);
    ret=register_chrdev_region(drv_prvdata.device_number, 4,device_path);
    if(ret < 0)
    {
        printk(KERN_ALERT"\ncould not allocate device numbers\n");
        return -1;
    }

    i=0;
    while(i<4)
    {
        printk(KERN_INFO"device number <major>:<minor>=%d:%d\n",MAJOR(drv_prvdata.device_number+i),MINOR(drv_prvdata.device_number+i));
        
        cdev_init(&drv_prvdata.dev_data[i].kcal_dev,&fops); 
        
         //kcalc_dev to initialize with related file operations retuns nothing

        if(cdev_add(&drv_prvdata.dev_data[i].kcal_dev,drv_prvdata.device_number+i,1) < 0)
        {
            j=0;
            while(j<i)
            {
                unregister_chrdev_region(drv_prvdata.device_number+j,1);
                j++;
            }
            printk(KERN_ALERT "\ncould not add devices to kernel\n");
            return -1;
        }

        printk(KERN_INFO "\nadded devices to kernel\n");

        i++;

    }   
    return 0;
}


int _strncmp(const char* str1,const char* str2,size_t n)
{
    size_t i=0;
    while(i<n && str1[i]==str2[i])
    {
        i++;
    }
    if(i==n)
    {
        return 0;
    }
    else if(str1[i] > str2[i])
    {
        return str1[i]-str2[i];
    }
    return str2[i]-str1[i];
}


static int calc_open(struct inode *inode, struct file *filp)
{
    unsigned int minor_num;
    struct dev_private_data* devc_prv_data;
    printk(KERN_INFO"Kernel Open Call\n");
    
    //finding which device is opened
    minor_num= MINOR(inode -> i_rdev);
    printk("minor number for which driver invoked %d\n",minor_num);

    //getting device private data

    devc_prv_data=container_of(inode->i_cdev,struct dev_private_data,kcal_dev);
    filp->private_data=devc_prv_data;


    if(_strncmp(devc_prv_data->name,"ADD",3)==0)
    {
        //devc_prv_data->kcal_dev.ops->read=Add_Read;
        //devc_prv_data->kcal_dev.ops->write=Add_Write;
        fops.read=Add_Read;
        fops.write=Add_Write;
    }
    
    else if(_strncmp(devc_prv_data->name,"SUB",3)==0)
    {
        //devc_prv_data->kcal_dev.ops->read=Sub_Read;
        //devc_prv_data->kcal_dev.ops->write=Sub_Write;

        fops.read=Sub_Read;
        fops.write=Sub_Write;
    }
    
    else if(_strncmp(devc_prv_data->name,"MUL",3)==0)
    {
        //devc_prv_data->kcal_dev.ops->read=Mul_Read;
        //devc_prv_data->kcal_dev.ops->write=Mul_Write;

        fops.read=Mul_Read;
        fops.write=Mul_Write;
    }
    else if(_strncmp(devc_prv_data->name,"DIV",3)==0)
    {
        //devc_prv_data->kcal_dev.ops->read=Div_Read;
        //devc_prv_data->kcal_dev.ops->write=Div_Write;

        fops.read=Div_Read;
        fops.write=Div_Write;
    }

    devc_prv_data->kcal_dev.ops=&fops;
    

    return 0;
}

static ssize_t Add_Write(struct file *filp, const char *buf, size_t len, loff_t * off)
{
    ssize_t retvalue=-1;
    struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;

    unsigned long result=0;


    //Returns number of bytes that could not be copied.
    result=copy_from_user((int*)pr_dvc_prvt_data->buffer,(char*)buf,len);
    
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

static ssize_t Add_Read(struct file *filp, char* buf, size_t len, loff_t * off)
{
    struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;
    ssize_t retvalue=-1;

    int _add=0;
    unsigned long result;

    if(*off + len > data_size)
    {
        len=data_size - *off;
    }
    

    printk(KERN_INFO "Kernel Add READ Call\n");

    printk(KERN_INFO"1st & 2nd operand in read %d %d\n",pr_dvc_prvt_data->buffer[0],pr_dvc_prvt_data->buffer[1]);
    
    _add=pr_dvc_prvt_data->buffer[0]+pr_dvc_prvt_data->buffer[1];
    
    printk(KERN_INFO"copying data to kernel space from user space");
    

    /*Returns number of bytes that could not be copied.*/
    result=copy_to_user(buf,&_add,len);

    printk("copied buffer %s\n",buf);

    
    if(result==0)
    {
        *off+=len;
        retvalue=len;
    }
    else if(result>0)
    {
        retvalue=len-result;
    }
    return retvalue;
}



static ssize_t Sub_Write(struct file *filp, const char *buf, size_t len, loff_t * off)
{
    struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;

    ssize_t retvalue=-1;

    unsigned long result=0;

    printk(KERN_INFO "Kernel Add WRITE Call\n");
    //Returns number of bytes that could not be copied.
    result=copy_from_user((int*)pr_dvc_prvt_data->buffer,(char*)buf,len);
    
    if(result >= 0)
    {
        retvalue=len-result;
        return retvalue;
    }
    printk(KERN_ALERT"Error writing Data\n");
    return retvalue;
}

static ssize_t Sub_Read(struct file *filp, char* buf, size_t len, loff_t * off)
{
     struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;

    ssize_t retvalue=-1;

    int _sub=0;
    unsigned long result;

    if(*off + len > data_size)
    {
        len=data_size - *off;
    }

    printk(KERN_INFO "Kernel SUB READ Call\n");

    printk(KERN_INFO"1st & 2nd operand in read %d %d\n",pr_dvc_prvt_data->buffer[0],pr_dvc_prvt_data->buffer[1]);
    
    _sub=pr_dvc_prvt_data->buffer[0]-pr_dvc_prvt_data->buffer[1];
    printk(KERN_INFO"result after subtraction %d\n",_sub);
    
    printk(KERN_INFO"copying %d bytes to  user space from kernel space\n",len);
    

    /*Returns number of bytes that could not be copied.*/
    result=copy_to_user(buf,&_sub,len);


    if(result == 0)
    {
        *off+=len;
        retvalue=len;
    }
    if(result>=0)
    {
        retvalue=len-result;
    }
    
    return retvalue;
}


static ssize_t Mul_Write(struct file *filp, const char *buf, size_t len, loff_t * off)
{
     struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;
    ssize_t retvalue=-1;


    unsigned long result=0;

    printk(KERN_INFO "Kernel Mul Write Call\n");

    //Returns number of bytes that could not be copied.
    result=copy_from_user((int*)pr_dvc_prvt_data->buffer,(char*)buf,len);
    
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

static ssize_t Div_Write(struct file *filp, const char *buf, size_t len, loff_t * off)
{
     struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;

    ssize_t retvalue=-1;


    unsigned long result=0;

    printk(KERN_INFO "Kernel Div Write Call\n");

    //Returns number of bytes that could not be copied.
    result=copy_from_user((int*)pr_dvc_prvt_data->buffer,(char*)buf,len);
    
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


static ssize_t Mul_Read(struct file *filp, char* buf, size_t len, loff_t * off)
{

     struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;


    ssize_t retvalue=-1;

    int _mul=0;
    unsigned long result;

    if(*off + len > data_size)
    {
        len=data_size - *off;
    }
    

    printk(KERN_INFO "Kernel Mul READ Call\n");

    printk(KERN_INFO"1st & 2nd operand in read %d %d\n",pr_dvc_prvt_data->buffer[0],pr_dvc_prvt_data->buffer[1]);
    
    _mul=pr_dvc_prvt_data->buffer[0]*pr_dvc_prvt_data->buffer[1];
    
    printk(KERN_INFO"copying data to kernel space from user space");
    

    /*Returns number of bytes that could not be copied.*/
    result=copy_to_user(buf,&_mul,len);


    printk("copied buffer %s\n",buf);

    if(result == 0)
    {
        *off+=len;
        retvalue=len;
    }
    else if(result>0)
    {
        retvalue=len-result;
    }

    return retvalue;
}

static ssize_t Div_Read(struct file *filp, char* buf, size_t len, loff_t * off)
{
    struct dev_private_data* pr_dvc_prvt_data=(struct dev_private_data*)filp->private_data;

    ssize_t retvalue=-1;

    int _div=0;
    unsigned long result;

    if(*off + len > data_size)
    {
        len=data_size - *off;
    }
    

    printk(KERN_INFO "Kernel Div READ Call\n");

    printk(KERN_INFO"1st & 2nd operand in read %d %d\n",pr_dvc_prvt_data->buffer[0],pr_dvc_prvt_data->buffer[1]);
    
    
    _div=pr_dvc_prvt_data->buffer[0]/pr_dvc_prvt_data->buffer[1];;
    
    printk(KERN_INFO"copying data to kernel space from user space");
    

    /*Returns number of bytes that could not be copied.*/
    result=copy_to_user(buf,&_div,len);

    printk("copied buffer %s\n",buf);

    if(result == 0)
    {
        *off+=len;
        retvalue=len;
    }
    if(result>=0)
    {
        retvalue=len-result;
    }
    
    return retvalue;
}


static int  calc_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"resetting array index in release call\n");
    arr_index=0;
    return 0;
}


static void __exit Calc_device_release(void)
{
    size_t i;
    unregister_chrdev_region(drv_prvdata.device_number,4);
    printk(KERN_INFO "unregistered character device successfully\n");
    i=0;
    while(i<4)
    {
        cdev_del(&drv_prvdata.dev_data[i].kcal_dev);
        i++;
    }
    printk(KERN_INFO "deleted  character devices from kernel\n");
    

}

module_init(Calc_device_initialization);
module_exit(Calc_device_release);
