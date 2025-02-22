SRCS =
SRCS += my_key_test.c
SRCS += my_led_test.c
SRCS += my_wifi_test.c
SRCS += my_net0_test.c
SRCS += my_uart_test.c
SRCS += my_otgdevice_test.c

SRCS += device_test.c
SRCS += net_tran_msg_test.c

OBJS = $(SRCS:.c=.o)
TARGETS = $(SRCS:.c=)
CC = arm-linux-gnueabihf-gcc
all: $(TARGETS)

%: %.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGETS)
