
obj-m : AVG/avg.o ADD/add.o

KDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PD) modules
install:
	$(MAKE) -C $(KDIR) M=$(PD) modules_install
	depmod -a
clean:
	$(MAKE) -C $(KDIR) M=$(PD) clean


