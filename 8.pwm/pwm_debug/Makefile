TEXT_BASE = 0x00000000

all : pwm.bin
pwm.bin : start.S main.c
	arm-linux-gcc  -g -c -o start.o start.S 
	arm-linux-gcc  -g -c -o main.o main.c
	arm-linux-ld -Ttext $(TEXT_BASE) -g  start.o main.o -o pwm_elf
	arm-linux-objcopy -O binary -S pwm_elf pwm.bin
	arm-linux-objdump -D -m arm  pwm_elf > pwm.dis
clean:
	rm -f pwm.dis pwm.bin pwm_elf *.o
