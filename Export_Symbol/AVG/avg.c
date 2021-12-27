#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/stat.h>
#include "add.h"

int avg_arr[5]={0};
int count=5;

module_param_array(avg_arr,int,&count,S_IRUGO);

static int __init avg_of_num_init(void)
{
	size_t res=add_from_array(avg_arr);

	printk("sum of 5 numbers is %d\n",res);
	res/=count;
	printk("avg is %d\n",res);
	return 0;
}
static void __exit avg_of_num_cleanup(void)
{
	printk("cleaning up avg function\n");
}

module_init(avg_of_num_init);
module_exit(avg_of_num_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("murthuja");
MODULE_DESCRIPTION("module to get average of five numbers\n");

