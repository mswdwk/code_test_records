obj-m:=kernel_mod_list.o    
KDIR:= /lib/modules/$(shell uname -r)/build
PWD:= $(shell pwd) 

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
