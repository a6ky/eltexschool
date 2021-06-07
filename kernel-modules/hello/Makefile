TARGET = hello
obj-m += $(TARGET).o

KERNELDIR := /usr/src/linux-headers-$(shell uname -r)

all:
	sudo make -C $(KERNELDIR) M=$(PWD) modules

clean:
	sudo make -C $(KERNELDIR) M=$(PWD) clean