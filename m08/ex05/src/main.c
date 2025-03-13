/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 13:51:21 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

uint8_t		g_led_select = 0; /* Led select */
uint8_t 	g_color = 0; /* color select */
uint32_t	g_led_1 = 0xFF000000;
uint32_t	g_led_2 = 0xFF000000;
uint32_t	g_led_3 = 0xFF000000;

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
	adc_init();
	button_init();
	uart_init();
	spi_init_master();
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
/**
 * @brief Set value
 *
 * @param adc -- ADC readed value
 */
void	set_led_value(const uint8_t adc) {
	uint32_t	mask = ~(0 | ((uint32_t)255 << (8 * (2 - g_color))));
	uint32_t	color = (uint32_t)adc << (8 * (2 - g_color));

	if (g_led_select == 0)
		g_led_1 = ((g_led_1 & mask) | color);
	if (g_led_select == 1)
		g_led_2 = ((g_led_2 & mask) | color);
	if (g_led_select == 2)
		g_led_3 = ((g_led_3 & mask) | color);
	spi_set_led(g_led_1, g_led_2, g_led_3);
}

int main() {
	uint8_t	adc = 0;
	uint8_t	b1_state = 1;
	uint8_t	b2_state = 1;
	uint8_t	b1_last_state = 1;
	uint8_t	b2_last_state = 1;


	init();
	spi_set_led(0, 0, 0);
	while (1) {
		b1_state = read_button(PIND, BUTTON1);
		b2_state = read_button(PIND, BUTTON2);
		if (b1_state == 0 && b1_last_state == 1) { /* Button 1 pressed */
			_delay_ms(20);
			g_color = (g_color + 1) % 3; /* Change color ch */
			b1_last_state = 0;
			continue ;
		}
		if (b2_state == 0 && b2_last_state == 1) { /* Button 2 pressed */
			_delay_ms(20);
			g_color = 0; /* Modify RED */
			g_led_select = (g_led_select + 1) % 3; /* Change led */
			b2_last_state = 0;
			continue ;
		}
		if (b1_last_state == 0 && b1_state) {
			_delay_ms(20);
			b1_last_state = 1;
		}
		if (b2_last_state == 0 && b2_state) {
			_delay_ms(20);
			b2_last_state = 1;
		}
		adc = adc_read(0);
		set_led_value(adc);
		_delay_ms(100);
	}
	
	return (0);
}
