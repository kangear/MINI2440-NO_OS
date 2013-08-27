all : lcd.bin
lcd.bin : start.S  main.c nand_cp.c sunflower.c
	arm-linux-gcc -g -c -o start.o start.S
	arm-linux-gcc -g -c -o main.o main.c
	arm-linux-gcc -g -c -o nand_cp.o nand_cp.c
	arm-linux-gcc -g -c -o sunflower.o sunflower.c
	arm-linux-ld -Tlcd.lds -Ttext 0x33F80000 start.o main.o nand_cp.o sunflower.o -o lcd_elf
	arm-linux-objcopy -O binary -S lcd_elf lcd.bin
	arm-linux-objdump -D -m arm  lcd_elf > lcd.dis
clean:
	rm -f   lcd.dis lcd.bin lcd_elf *.o
