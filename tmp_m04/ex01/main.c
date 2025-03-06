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

volatile uint8_t count_dir = 0;

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
	DDRB |= (1 << DDB1); /* Set PB0 as output */

	/* TIMER0 INIT */
	TCCR0A |= (1 << WGM01); /* CTC */
	TCCR0B |= (1 << CS02) | (0 << CS00); /* 64 prescaler */
	OCR0A = 120; /* Duty cycle value 50% of 0xFF */
	TIMSK0 |= (1 << OCIE0A); /* Enable interrupt */

	/* TIMER1 INIT */
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); /* Mode Fast PWM, TOP ICR1
											 * Clear OC1A on compare match, set OC1A at BOTTOM */
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10); /* Mode Fast PWM, TOP ICR1
														  * No prescaler */
	ICR1 = 0xFF; /* Max TOP value for timer1 */
	OCR1A = 0x0; /* Duty cycle value 0% of 0xFF */
}

ISR(TIMER0_COMPA_vect) {
	if (count_dir == 0) {
		OCR1A++;
		if (OCR1A == ICR1) {
			count_dir = 1;
		}
	} else {
		OCR1A--;
		if (OCR1A == 0x0) {
			count_dir = 0;
		}
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
