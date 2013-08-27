all : sdram.bin
sdram.bin : head.S  leds.c nand_cp.c
	arm-linux-gcc -g -c -o head.o head.S
	arm-linux-gcc -g -c -o leds.o leds.c
	arm-linux-gcc -g -c -o nand_cp.o nand_cp.c
	arm-linux-ld -Ttext 0x33F80000 head.o leds.o nand_cp.o -o sdram_elf
	arm-linux-objcopy -O binary -S sdram_elf sdram.bin
	arm-linux-objdump -D -m arm  sdram_elf > sdram.dis
clean:
	rm -f   sdram.dis sdram.bin sdram_elf *.o
