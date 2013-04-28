obj-m := cdcacm.o
KVERSION := $(shell uname -r)
PWD := $(shell pwd)
#export CFLAGS = "-DPATCH_DISABLED"

all:
	$(MAKE) -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
