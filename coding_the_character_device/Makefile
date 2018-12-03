obj-m :=solidusmodule.o

KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

clear:
	rm -rf *.o *.ko *.mod *.symvers *.order *~
