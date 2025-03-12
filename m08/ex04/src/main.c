/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 00:13:06 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/io.h>
#include <stdint.h>

uint32_t	led_1 = 0;
uint32_t	led_2 = 0;
uint32_t	led_3 = 0;

/** *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 *    - Schem at Page 12
 *    - TIMER0 at Page 102-120
 *    - Interrupts at page 66 - 78
 *    - External Interrupts at Pages 79 - 84
 */

/* ************************************************************************** */
/*                                    INIT                                    */
/* ************************************************************************** */

/**
 * @brief Initialize mmcu
 */
void	init(void) {
	/* Timer setup */
	TCCR0A |= (1 << WGM01); /* Set CTC mode */
	TCCR0B |= (1 << CS02) | (1 << CS00); /* Prescaler 1024 */
	OCR0A = 92; /* Set compare limit */

	uart_init();
	spi_init_master();

	/* Interrupts Enable */
	SREG |= (1 << 7);
}

/* ************************************************************************** */
/*                                   COLOR                                    */
/* ************************************************************************** */
void set_rgb(uint32_t r, uint32_t g, uint32_t b) {
	spi_set_led(0xFF000000 | r << 16 | g << 8 | b,
				0xFF000000 | g << 16 | b << 8 | r,
				0xFF000000 | b << 16 | r << 8 | g);
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else { pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

/* ************************************************************************** */
/*                                  INTERRUPTS                                */
/* ************************************************************************** */
ISR(TIMER0_COMPA_vect) {
	static uint8_t	color = 0;

	wheel(color);
	color = color + 1 % 255;
}

/* ************************************************************************** */
/*                                    TEST                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

void	exec(unsigned char *buff) {
	uint8_t	led;
	uint32_t	val = parse(buff);

	if (val == 0 || val == 0xFF)
		return ;
	led = val & 0xF;
	if (led == 6)
		led_1 = (0xFF000000 | (val >> 8));
	if (led == 7)
		led_2 = (0xFF000000 | (val >> 8));
	if (led == 8)
		led_3 = (0xFF000000 | (val >> 8));
	spi_set_led(led_1, led_2, led_3);
}

int main() {
	init();

	spi_set_led(0, 0, 0);
	// TIMSK0 |= (1 << OCIE0A); /* Enable interrupts on copmare match */
	unsigned char	buff[READ_SIZE];
	while (1) {
		ft_memset(buff, 0x00, READ_SIZE);
		uart_read_input(buff, READ_SIZE);
		exec(buff);
	}
	
	return (0);
}
