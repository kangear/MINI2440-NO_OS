TEXT_BASE = 0x00000000

all : clock.bin
clock.bin : start.S
	arm-linux-gcc  -g -c -o start.o start.S
	arm-linux-ld -Ttext $(TEXT_BASE) -g  start.o -o clock_elf
	arm-linux-objcopy -O binary -S clock_elf clock.bin
	arm-linux-objdump -D -m arm  clock_elf > clock.dis
clean:
	rm -f clock.dis clock.bin clock_elf *.o
