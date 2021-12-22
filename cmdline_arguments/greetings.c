#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Murthu");

static char* name="There";

module_param(name,charp,S_IRUGO);

static int __init greet_init(void)
{
    printk(KERN_ALERT "Hello %s\n",name);
    return 0;
}



static void __exit greet_exit(void)
{
    printk(KERN_ALERT,"BYE\n");
    
}

module_init(greet_init);
module_exit(greet_exit);
