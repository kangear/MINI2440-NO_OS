TEXT_BASE = 0x00000000

all : timer.bin
timer.bin : start.S main.c
	arm-linux-gcc  -g -c -o start.o start.S 
	arm-linux-gcc  -g -c -o main.o main.c
	arm-linux-ld -Ttext $(TEXT_BASE) -g  start.o main.o -o timer_elf
	arm-linux-objcopy -O binary -S timer_elf timer.bin
	arm-linux-objdump -D -m arm  timer_elf > timer.dis
clean:
	rm -f timer.dis timer.bin timer_elf *.o
