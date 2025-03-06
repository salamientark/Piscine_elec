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

#define DEBOUNCE_TIME 3

volatile uint8_t debounce_flag = 0;
volatile uint8_t press_dir = 0;

/** *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 *    - Schem at Page 12
 *    - TIMER0 at Page 102-120
 *    - Interrupts at page 66 - 78
 *    - External Interrupts at Pages 79 - 84
 */

/* ************************************************************************** */
/*                                   INIT                                     */
/* ************************************************************************** */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	/* LED INIT */
	DDRB |= (1 << DDB0); /* Set PB0 as output */
	PORTB &= ~(1 << PB0); /* Set PB0 to LOW */

	/* TIMER INIT */
	/* This is an attempt to solve bouncing */
	TCCR0A |= (1 << WGM01); /* CTC mode */
	TCCR0B |= (1 << CS02) | (1 << CS00); /* Prescaler 1024 */
	OCR0A = 255; /* Compare value 33ms | 30Hz*/

	/* INTERRUPS INIT */
	EICRA |= (1 << ISC01); /* Falling edge of INT0 generate interrupts
							* When button is pressed generate interrupts*/
	EIMSK |= (1 << INT0); /* Activate interrupt on INT0 */
}

ISR(INT0_vect) {
	if (!debounce_flag) {
		debounce_flag = 1;
		TCNT0 = 0; /* Reset Timer0 */
		TIMSK0 |= (1 << OCIE0A); /* Set interrupt flag */

		press_dir = (EICRA & (1 << ISC00)); /* Get current press dir (pressing / releasing) */
	}
}

ISR(TIMER0_COMPA_vect) {
    static volatile uint8_t count = 0;
    count++;
    if (count >= DEBOUNCE_TIME) {
        debounce_flag = 0;
        count = 0;
        TIMSK0 &= ~(1 << OCIE0A); /* Disable interrupt */
		if (!press_dir)
			PORTB ^= (1 << PB0); /* Toggle LED */
		EICRA ^= (1 << ISC00); /* Toggle interrupt edge */
    }
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();
	sei(); /* Enable global interrupts */

	while (1) {}

	return (0);
}
