TEXT_BASE = 0x00000000

all : led_on_c.bin
led_on_c.bin : start.S  led_on_c.c
	arm-linux-gcc  -g -c -o start.o start.S
	arm-linux-gcc  -g -c -o led_on_c.o led_on_c.c
	arm-linux-ld -Ttext $(TEXT_BASE) -g  start.o led_on_c.o -o led_on_c_elf
	arm-linux-objcopy -O binary -S led_on_c_elf led_on_c.bin
	arm-linux-objdump -D -m arm  led_on_c_elf > led_on_c.dis
clean:
	rm -f led_on_c.dis led_on_c.bin led_on_c_elf *.o
