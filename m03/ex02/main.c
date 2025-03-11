/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */ /*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/04 22:58:14 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/** *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 */

/* ************************************************************************** */
/*                                   INIT                                     */
/* ************************************************************************** */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	/* RGB LED INIT */
	DDRD = 0b01101000; /// Set RGB LED as outputs in the DATA_DIRECTION_REGISTER
}

/* ************************************************************************** */
/*                                  RGB_LED                                   */
/* ************************************************************************** */
void init_rgb() {
	/* TIMER0 Setup TO Hz */
	TCCR0A = 0b10100011; /* OC0A and OC0B in Inverting mode
						  * and set Mode To Fast PWM with TOP = 0xFF
						  */
	TCCR0B = 0b00000001; /* Set no prescaler */

	/* TIMER2 Setup TO Hz */
	TCCR2A = 0b00100011; /* OC2A in Inverting mode
						  * and set Mode To Fast PWM with TOP = 0xFF
						  */
	TCCR2B = 0b00000001; /* Set prescaler no prescaler */

}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
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
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();
	init_rgb();

	while (1) {
		for (volatile uint8_t i = 0; i < 255; i++) {
			wheel(i);
			_delay_ms(10);
		}
	}
	return (0);
}
