TEXT_BASE = 0x00000000

all : sdram.bin
sdram.bin : start.S
	arm-linux-gcc  -g -c -o start.o start.S
	arm-linux-ld -Ttext $(TEXT_BASE) -g  start.o -o sdram_elf
	arm-linux-objcopy -O binary -S sdram_elf sdram.bin
	arm-linux-objdump -D -m arm  sdram_elf > sdram.dis
clean:
	rm -f sdram.dis sdram.bin sdram_elf *.o
