objs := head.o init.o  main.o

all: int.bin
int.bin: $(objs)
	arm-linux-ld -Ti2c.lds -o int_elf $^
	arm-linux-objcopy -O binary -S int_elf $@
	arm-linux-objdump -D -m arm int_elf > int.dis
	
%.o:%.c
	arm-linux-gcc -g -Wall  -c -o $@ $<

%.o:%.S
	arm-linux-gcc -g -Wall  -c -o $@ $<

clean:
	rm -f int.bin int_elf int.dis *.o		
	