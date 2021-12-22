PWD := $(shell pwd)
KERNEL_DIR = /lib/modules/$(shell uname -r)/build

obj-m := add.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules_install
	depmod -a
