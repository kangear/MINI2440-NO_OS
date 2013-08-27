#Makefile

CROSS_COMPILE:=arm-linux-
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

CFLAGS = -g -Wall -O2 -c

TEXT_BASE = 0x000000

objs := start.o main.o

all : uart.bin
uart.bin: $(objs)
	$(LD) -Ttext $(TEXT_BASE) -o uart_elf $^
	$(OBJCOPY) -O binary -S uart_elf $@
	$(OBJDUMP) -D -m arm uart_elf > uart.dis
	
%.o:%.c
	$(CC)  $(CFLAGS) -o $@ $<

%.o:%.S
	$(CC)  $(CFLAGS) -o $@ $<

clean:
	rm -f uart.bin uart_elf uart.dis *.o		
	