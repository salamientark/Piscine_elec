### CHIP CONFIG ###
MMCU := atmega328p
F_CPU := 16000000UL

### COMPILATION FLAGS ###
CC := avr-gcc
OBJCOPY := avr-objcopy
AVRDUDE := avrdude

PROGRAMMER := arduino
UART_BAUDRATE := 115200

### RULES ###
all : hex flash


hex : main.c
	$(CC) -O1 -mmcu=$(MMCU) -Wall -Wextra -Werror -D F_CPU=$(F_CPU) -D UART_BAUDRATE=$(UART_BAUDRATE)  main.c -o main.bin	
	$(OBJCOPY) -O ihex main.bin main.hex

flash: hex
	$(AVRDUDE) -c $(PROGRAMMER) -p m328p -b $(UART_BAUDRATE) -U flash:w:main.hex -P /dev/ttyUSB0

clean :
	rm main.hex main.bin
