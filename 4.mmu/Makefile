objs := start.o init.o leds.o

all : mmu.bin
mmu.bin : $(objs)
	arm-linux-ld -Ttext 0x00000000 -o mmu_elf $^
	arm-linux-objcopy -O binary -S mmu_elf $@
	arm-linux-objdump -D -m arm mmu_elf > mmu.dis
	
%.o:%.c
	arm-linux-gcc -g -Wall -O2 -c -o $@ $<

%.o:%.S
	arm-linux-gcc -g -Wall -O2 -c -o $@ $<

clean:
	rm -f mmu.bin mmu_elf mmu.dis *.o		
	
