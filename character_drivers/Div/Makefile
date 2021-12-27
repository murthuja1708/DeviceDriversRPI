PWD := $(shell pwd)
KERNEL_DIR = /lib/modules/$(shell uname -r)/build

obj-m := div_driver.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf /lib/modules/$(shell uname -r)/extra/
