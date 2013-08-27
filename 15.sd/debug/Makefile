all : lcd.bin
lcd.bin : start.S  main.c nand_cp.c sdi.c
	arm-linux-gcc -g -c -o start.o start.S
	arm-linux-gcc -g -c -o main.o main.c
	arm-linux-gcc -g -c -o nand_cp.o nand_cp.c
	arm-linux-gcc -g -c -o sdi.o sdi.c
	arm-linux-ld -Tsd.lds -Ttext 0x33F80000 start.o main.o nand_cp.o sdi.o -o sd_elf
	arm-linux-objcopy -O binary -S sd_elf sd.bin
	arm-linux-objdump -D -m arm  sd_elf > sd.dis
clean:
	rm -f   sd.dis sd.bin sd_elf *.o
