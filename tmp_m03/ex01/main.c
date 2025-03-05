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
#include <util/delay_basic.h>

/* Define RGB LED COLOR CODE */
#define RGB_LED_RED		0b00100000
#define RGB_LED_GREEN	0b01000000
#define RGB_LED_BLUE	0b00001000

#define RGB_LED_YELLOW	0b01100000
#define RGB_LED_CYAN	0b01001000
#define RGB_LED_MAGENTA	0b00101000
#define RGB_LED_WHITE	0b01101000

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
	/* TIMER10 Setup */
	TCCR1A = 0b00000000; /* Set CTC mode */
	TCCR1B = 0b00001101; /* Set prescaler to 1024 */
	OCR1A = (uint16_t) (F_CPU / 1024 - 1); /* Set compare value */
	TIMSK1 = 0b00000010; /* Enable interrupt when comapre with OCCR0A occur */
	
	/* RGB LED INIT */
	DDRD = 0b01101000; /// Set RGB LED as outputs in the DATA_DIRECTION_REGISTER
	PORTD |= (1 << PD5); /* Set RED LED to RED */
}

/* ************************************************************************** */
/*                                 INTERUPTS                                  */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();

	while (1) {
		PORTD = RGB_LED_RED; /* LED RED */
		_delay_ms(1000);
		PORTD = RGB_LED_GREEN; /* LED GREEN */
		_delay_ms(1000);
		PORTD = RGB_LED_BLUE; /* LED BLUE */
		_delay_ms(1000);
		PORTD = RGB_LED_YELLOW; /* LED YELLOW */
		_delay_ms(1000);
		PORTD = RGB_LED_CYAN; /* LED CYAN */
		_delay_ms(1000);
		PORTD = RGB_LED_MAGENTA; /* LED MAGENTA */
		_delay_ms(1000);
		PORTD = RGB_LED_WHITE; /* LED WHITE */
		_delay_ms(1000);
	}
	return (0);
}
