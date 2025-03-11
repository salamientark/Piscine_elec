### CHIP CONFIG ###
MMCU := atmega328p
F_CPU := 16000000UL

### COMPILATION FLAGS ###
CC := avr-gcc
OBJCOPY := avr-objcopy
AVRDUDE := avrdude

PROGRAMMER := arduino
UART_BAUDRATE := 115200
### COMPILATION FLAGS ###

### GLOBAL CONFIG ###
PROJECT := setup
PROJECT_DIR := ./

SRC_DIR := ./
SRC_FILE := main.c

### OBJ FILE ###
# OBJ_DIR := .obj/
# OBJ_FILE := $(addprefix $(OBJ_DIR), $(SRC_FILE:.c=.bin))


### RULES ###
all : hex flash


hex : main.c
	$(CC) -O1 -mmcu=$(MMCU) -Wall -Wextra -Werror -D F_CPU=$(F_CPU) -D UART_BAUDRATE=$(UART_BAUDRATE)  main.c -o main.bin	
	$(OBJCOPY) -O ihex main.bin main.hex

flash: hex
	$(AVRDUDE) -c $(PROGRAMMER) -p m328p -b $(UART_BAUDRATE) -U flash:w:main.hex -P /dev/ttyUSB0
	avr-gcc -mmcu=atmega328p -Wall -Wextra -Werror -D F_CPU=16000000UL  main.c -o main.bin	
	avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -c arduino -p m328p -b 115200 -U flash:w:main.hex -P /dev/ttyUSB0

clean :
	rm main.hex main.bin
