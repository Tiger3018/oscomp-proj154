# SPDX-License-Identifier: GPL-3.0-or-later
VERSION = 0
PATCHLEVEL = 0
SUBLEVEL = 1
# EXTRAVERSION = -test
MODULE_NAME = DKMS_Module

# *DOCUMENTATION*
# To see a list of typical targets execute "make help"
# More info can be located in ./README
# Comments in this file are targeted only to the developer, do not
# expect to learn how to build the kernel reading this file.

$(if $(filter __%, $(MAKECMDGOALS)), \
	$(error targets prefixed with '__' are only for internal use))

# From Realtek Module Makefile
MAKE = make
SUBARCH := $(shell uname -m | sed -e s/i.86/i386/)
ARCH ?= $(SUBARCH)
CROSS_COMPILE ?=
KVER := $(shell uname -r)
KSRC := /lib/modules/$(KVER)/build
INSTALL_PREFIX ?=

$(MODULE_NAME)-y += src/proc.o

obj-m := $(MODULE_NAME).o

# That's our default target when none is given on the command line
PHONY := __all

all: modules

__all: modules

.PHONY: modules clean

modules:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KSRC) M=$(shell pwd) modules

clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KSRC) M=$(shell pwd) clean
	rm -fr */*.mod.c */*.mod */*.o */.*.cmd
	rm -f *.mod.c *.o .*.cmd *.ko
	rm -f Module.symvers Module.markers modules.order
	rm -fr .tmp_versions

help:
	@echo "options :"
	@echo "modules		build this module"
	@echo "clean		clean"
