#include<linux/module.h>

static int __init helloworld_init(void)
{
    printk(KERN_INFO "HELLO WORLD\n");
    return 0;
}

static void __exit helloword_cleanup(void)
{
    printk(KERN_INFO "BYE WORLD\n");
}

module_init(helloworld_init);
module_exit(helloword_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("murthuja");
MODULE_DESCRIPTION("simple hello world kernel module");

