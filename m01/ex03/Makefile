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
	avr-gcc -O1 -mmcu=atmega328p -Wall -Wextra -Werror -D F_CPU=16000000UL  main.c -o main.bin	
	avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -c arduino -p m328p -b 115200 -U flash:w:main.hex -P /dev/ttyUSB0

clean :
	rm main.hex main.bin
