/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */ /*   main.c                                             :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
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
* @brief Initialize RGB LED timer
*/
void timer_init(void) {
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

/**
* @brief Initialize ADC registers
*/

 void	adc_init(void) {
	ADMUX = (1 << REFS0) | (1 << ADLAR); /* Set reference voltage to AVCC and select ADC0 */
	ADCSRA = (1 << ADEN) /* Enable ADC */
			| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); /* Prescaler = 128 */
	DIDR0 = 0b00111110; /* Enable digital input on ADC0 */
}


/**
 * @brief Initialize DATA_DIRECTION registers
 */
 void init(void) {
	/* D1..4 LED as output */
	DDRB = 0b00010111;

	/* RGB LED as output */
	DDRD = 0b01101000; /// Set RGB LED as outputs in the DATA_DIRECTION_REGISTER

	timer_init();
	adc_init();
}

/* ************************************************************************** */
/*                                  RGB_LED                                   */
/* ************************************************************************** */

/**
* @brief Set the RGB LED to a specific color
*
* @param r -- Red value
* @param g -- Green value
* @param b -- Blue value
*/
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
/*                                   UART                                     */
/* ************************************************************************** */

/**
* @brief Read ADC value
*
* @param ch - channel to read
*
* @return ADC value
*/
 uint8_t	adc_read(uint8_t ch) {
	ch &= 0b00000111; /* Protection sclaing ch to 0:7 
					   * ch 8 is internal tem sensor
					   */
	ADMUX = (ADMUX & 0b11111000) | ch; /* Enable only active channel */
	ADCSRA |= (1 << ADSC); /* Start conversation */
	while (ADCSRA & (1 << ADSC)) {} /* Wait until ADC conversation complete */
	return (ADCH);
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

void display_value(const uint8_t val) {
	PORTB = ((val == 255) << PB4)
			| ((val > 192) << PB2)
			| ((val > 128) << PB1)
			| ((val > 64) << PB0);
}

int main() {
	init();
	sei(); /* Enable global interrupts */

	uint8_t	adc;
	while (1) {
		adc = adc_read(0);
		wheel(adc);
		display_value(adc);
	}

	return (0);
}
