CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

INCLUDEDIR 	:= $(shell pwd)/include
CFLAGS 		:= -g -Wall -O2 -fno-builtin
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR)

export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

objs := start.o main.o uart.o lib/libc.a

all : stdio.bin
stdio.bin: $(objs)
	#${LD} -Tstdio.lds -o stdio.elf $^
	${LD} -Ttext 0x30000000 -o stdio.elf $^
	${OBJCOPY} -O binary -S stdio.elf $@
	${OBJDUMP} -D stdio.elf > stdio.dis

.PHONY : lib/libc.a
lib/libc.a:
	cd lib; make; cd ..
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	make  clean -C lib
	rm -f *.bin *.elf *.dis *.o
	
