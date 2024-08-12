obj-m += module_id.o
CC = $(shell cat /proc/version | grep -oP 'gcc-\d+')
MODULE_NAME=module_id

all:
	make -C /lib/modules/$(shell uname -r)/build CC=$(CC) M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod $(MODULE_NAME).ko
	@echo "Checking kernel log:"
	@sudo dmesg -c
	@echo "Checking id module in kernel:"
	@lsmod | grep $(MODULE_NAME)

unload:
	sudo rmmod $(MODULE_NAME).ko
	@echo "Checking kernel log:"
	@sudo dmesg -c
	@echo "Checking id module in kernel:"
	@if lsmod | grep -q $(MODULE_NAME); then \
		echo "Module is still loaded in the system."; \
	else \
		echo "Module is not found in the system."; \
	fi