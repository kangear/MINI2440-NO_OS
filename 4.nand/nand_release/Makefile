all : sdram.bin
sdram.bin : start.S  leds.c nand_cp.c
	arm-linux-gcc -g -c -o start.o start.S
	arm-linux-gcc -g -c -o leds.o leds.c
	arm-linux-gcc -g -c -o nand_cp.o nand_cp.c
	arm-linux-ld -Ttext 0x33F80000 start.o leds.o nand_cp.o -o nand_elf
	arm-linux-objcopy -O binary -S nand_elf nand.bin
	arm-linux-objdump -D -m arm  nand_elf > nand.dis
clean:
	rm -f   nand.dis nand.bin nand_elf *.o
