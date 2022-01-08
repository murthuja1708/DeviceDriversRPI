#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fcntl.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>


#define NO_OF_DEVICES 4
#define data_size 4 //sizeof(int)

/*
struct dev_private_data {
    const char* name;
    int* buffer;
    unsigned size;
    //uint8_t perm;
    struct cdev kcal_dev;
};

struct driver_private_data{
    int total_devices;
    dev_t device_number;
    struct dev_private_data dev_data[NO_OF_DEVICES];
};
*/
void printfilep(struct file* filp);

static int calc_open(struct inode *inode, struct file *file);

static int  calc_release(struct inode *inode, struct file *file);

static ssize_t Add_Write(struct file *filp, const char __user *buf, size_t len, loff_t * off);
static ssize_t Add_Read(struct file *filp, char __user* buf, size_t len, loff_t * off);
static ssize_t Sub_Write(struct file *filp, const char __user *buf, size_t len, loff_t * off);
static ssize_t Sub_Read(struct file *filp, char __user*  buf, size_t len, loff_t * off);

static ssize_t Mul_Write(struct file *filp, const char __user *buf, size_t len, loff_t * off);

static ssize_t Div_Write(struct file *filp, const char __user *buf, size_t len, loff_t * off);


static ssize_t Mul_Read(struct file *filp, char __user *buf, size_t len, loff_t * off);
static ssize_t Div_Read(struct file *filp, char __user *buf, size_t len, loff_t * off);


