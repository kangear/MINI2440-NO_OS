all : start.S
	arm-linux-gcc -g -c -o start.o start.S
	arm-linux-gcc -g -c -o main.o main.c
	arm-linux-ld -Ttext 0x0000000 -g start.o main.o -o adc_elf
	arm-linux-objcopy -O binary -S adc_elf adc.bin
clean:
	rm -f   adc.bin adc_elf *.o
