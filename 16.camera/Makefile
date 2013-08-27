CC=arm-linux-gcc      
LD=arm-linux-ld

CFLAGS :=  -Wall -c -g

OBJCOPY=arm-linux-objcopy
OBJDUMP=arm-linux-objdump


objs := start.o lowlevel_init.o nand.o mmu.o mmulib.o interrupt.o  main.o  2440lib.o print.o lcd.o font_libs.o ov9650.o

all: $(objs)
        
	$(LD) -Bstatic -Tboot.lds -Ttext 0x30000000 $(objs) \
        -L/opt/FriendlyARM/toolschain/4.4.3/lib/gcc/arm-none-linux-gnueabi/4.4.3 \
        -L/opt/FriendlyARM/toolschain/4.4.3/arm-none-linux-gnueabi/sys-root/usr/lib  \
        -Map boot.map -o boot.elf --start-group -lgcc -lgcc_eh -lgcov -lc --end-group    
	arm-linux-objdump -D boot.elf > boot.dis
 
	$(OBJCOPY)  -O binary  boot.elf boot.bin 


%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<
%.o:%.S
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f *.bin *.elf  *.map *.o

