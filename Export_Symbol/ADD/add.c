#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/stat.h>
#include<linux/init.h>



int a[5]={1,2,3,4,5};
int count=5;

//module_param_array(a,int,&count,S_IRUGO);

int  add_from_array(int arr[])
{
	int res=0;
	int i=0;
	while(i<count)
	{
		res+=arr[i];
		i++;
	}
    return res;
}


EXPORT_SYMBOL(add_from_array);

static int __init add_init(void)
{
	printk("add %d\n",add_from_array(a));
	return 0;
}
static void __exit add_cleanup(void)
{
	printk("cleaning up add function\n");
}

module_init(add_init);
module_exit(add_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("murthuja");
MODULE_DESCRIPTION("module to add two numbers\n");

