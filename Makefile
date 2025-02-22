export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

CROSS_COMPILE ?= 
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)g++
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -O2 -g
CFLAGS += -I $(shell pwd)/include

LDFLAGS := -lts -lpthread -lfreetype -lm -lstdc++

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := test

obj-y += main.o
obj-y += UserInterface/
obj-y += SystemInterface_AL/
#obj-y += unitest/


all : start_recursive_build $(TARGET) DEVICE_TEST
	@echo $(TARGET) has been built!

start_recursive_build:
	make -C ./ -f $(TOPDIR)/Makefile.build

$(TARGET) : built-in.o
	$(CC) -o $(TARGET) built-in.o $(LDFLAGS)

DEVICE_TEST:
	make -C ./CoreLogic -f $(TOPDIR)/CoreLogic/Makefile
	
clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)
	rm -f $(shell find -name "my_*_test")

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	rm -f $(shell find -name "my_*_test")
	