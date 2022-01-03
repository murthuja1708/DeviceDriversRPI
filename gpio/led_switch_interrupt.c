#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>
#include<asm/uaccess.h>

#define DEBOUNCE_TIME 500

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Murthu");

static unsigned int gpioLED=1;
static unsigned int gpioSwitch=2;
static bool gpioVal=0;

unsigned long IRQFlags=IRQF_TRIGGER_RISING;
static int irq_number;

static irq_handler_t SwitchInterruptHandler(unsigned int irq,void* dev_id,struct pt_regs* regs);

static int  led_driver_initialization(void)
{
    int result;
    if(!gpio_is_valid(gpioLED))
    {
        printk(KERN_INFO"Invalid GPIO Led pin");
        return -ENODEV;
    }
    gpio_request(gpioLED,"GPIO_1");
    gpio_direction_output(gpioLED,1);
    gpio_export(gpioLED,false);

    if(!gpio_is_valid(gpioSwitch))
    {
        printk(KERN_INFO"Invalid GPIO Led pin");
        return -ENODEV;
    }
    gpio_request(gpioSwitch,"GPIO_2");
    gpio_direction_input(gpioSwitch);
    gpio_set_debounce(gpioSwitch,DEBOUNCE_TIME);
    gpio_export(gpioSwitch,false);

    irq_number=gpio_to_irq(gpioSwitch);

    result=request_irq(irq_number,
                        (irq_handler_t)SwitchInterruptHandler,
                        IRQFlags,
                        "LED-SWITCH_HANDLER",
                        NULL);

    return 0;
}



static void  led_driver_release(void)
{
    free_irq(irq_number,NULL);
    gpio_unexport(gpioLED);
    gpio_unexport(gpioSwitch);

    printk(KERN_INFO"unregistered LED and SWITCH pins\n");
}

static irq_handler_t SwitchInterruptHandler(unsigned int irq,void* dev_id,struct pt_regs* regs)
{
    printk(KERN_INFO"interrupt generated\n");
    gpioVal=!gpioVal;
    gpio_set_value(gpioLED,gpioVal);

    return (irq_handler_t)IRQ_HANDLED;
}

module_init(led_driver_initialization);
module_exit(led_driver_release);