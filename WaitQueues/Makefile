PWD := $(shell pwd)
KERNEL_DIR = /lib/modules/$(shell uname -r)/build

obj-m := driver.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules_install
	depmod -a

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf /lib/modules/$(shell uname -r)/extra/
