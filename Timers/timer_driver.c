#include<linux/init.h>
#include<linux/module.h>
#include<linux/fcntl.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/gpio.h>
#include<linux/timer.h>
#include<linux/jiffies.h>
#include "timer_driver.h"


unsigned int wait_time=3000;


dev_t calc_dev=0;
struct cdev kcalc_dev;

const char* device_path="/dev/timer_driver";

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Murthu");

static int  led_timer_driver_initialization(void);

static void  led_timer_driver_release(void);

static struct timer_list led_timer;

void timer_callback_led_toggle(struct timer_list* _tim);

static unsigned int gpioLED=1;
static bool gpioVal=0;

static int timer_open(struct inode* inode,struct file* filep);
static long int my_ioctl(struct file* filep,unsigned cmd,unsigned long argp);
static int timer_close(struct inode* inode,struct file* filep);


static struct file_operations fops={
    .owner      = THIS_MODULE,
    .open     = timer_open,
    .unlocked_ioctl  =my_ioctl,
    .release  = timer_close,
};

static int timer_open(struct inode* inode,struct file* filep)
{
    printk(KERN_INFO"Open call\n");
    
    return 0;
}

static int  led_timer_driver_initialization(void)
{
    calc_dev = MKDEV(501,1501);
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

    if(!gpio_is_valid(gpioLED))
    {
        printk(KERN_INFO"Invalid GPIO Led pin");
        return -ENODEV;
    }
    gpio_request(gpioLED,"GPIO_1");
    gpio_direction_output(gpioLED,0);
    gpio_export(gpioLED,false);

    timer_setup(&led_timer,timer_callback_led_toggle,0);

    //add_timer(&led_timer);
    //set up the timer interval to TIME_OUT macro
    int is_active=mod_timer(&led_timer,jiffies + msecs_to_jiffies(wait_time));
    if(is_active)
    {
        printk(KERN_INFO"timer firing failed\n");;
        return -1;
    }
    printk(KERN_INFO"timer active while initialization-->%d\n",is_active);

    

    //gpioVal=gpio_get_

    //set up timer to call callback function when timer expires
   
    return 0;
}

static long int my_ioctl(struct file* filep,unsigned cmd,unsigned long argp)
{
    long result;
    
    switch (cmd)
    {
    case set_periodicity:
        result=copy_from_user(&wait_time,(int*)argp,sizeof(wait_time));
        if(result == 0)
        {
            return result;        
        }
        else if(result > 0)
        {
            printk("Err while copying data\n");
        }
        break;
    
    case get_periodicity:
        result=copy_to_user((int*)argp,(int*)&wait_time,sizeof(wait_time));
        if(result>0)
        {
             printk("Err while getting periodicity while copying data\n");
        }
        break;
    
    }

    return result;
}

void timer_callback_led_toggle(struct timer_list* _tim)
{
    gpioVal=!gpioVal;
    gpio_set_value(gpioLED,gpioVal);
    int led_value=gpio_get_value(gpioLED);
    unsigned long jiffies_for_msecs=msecs_to_jiffies(wait_time);
    int is_active = mod_timer(&led_timer,jiffies+jiffies_for_msecs);
    if(is_active)
    {
        printk(KERN_INFO"Error while triggering\n");
        return;
    }

}

static int timer_close(struct inode* inode,struct file* filep)
{
    printk(KERN_INFO"Close call\n");
    
    return 0;
}


static void  led_timer_driver_release(void)
{
    
    del_timer(&led_timer);
    unregister_chrdev_region(calc_dev, 1);
     printk(KERN_INFO "unregistered character device successfully\n");
    cdev_del(&kcalc_dev);
    printk(KERN_INFO "deleted  character device from kernel\n");
    gpio_unexport(gpioLED);
    printk(KERN_INFO"unregistered LED and SWITCH pins\n");
    
    
}

module_init(led_timer_driver_initialization);
module_exit(led_timer_driver_release);

