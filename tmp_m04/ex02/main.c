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

volatile uint8_t b1_debounce = 0;
volatile uint8_t b1_press_dir = 0;
// volatile uint8_t b2_debounce = 0;
volatile uint8_t b2_last_state = 1;
volatile uint8_t nbr = 0;

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
	DDRB |= (1 << DDB4) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0); /* Set PB0,
																	* PB1, PB2
																	* PB4
																	* as output */
	PORTB &= ~((1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0)); /* Set PB0,
																	* PB1, PB2,
																	* PB4
																	* to OFF */

	/* TIMER0 INIT */
	/* This is an attempt to solve bouncing */
	TCCR0A |= (1 << WGM01); /* CTC mode */
	TCCR0B |= (1 << CS02) | (1 << CS00); /* Prescaler 1024 */
	OCR0A = 255; /* Compare value 33ms | 30Hz*/

	/* TIMER2 INIT */
	/* Used to solve bouncing */
	TCCR2A |= (1 << WGM21); /* CTC mode */
	TCCR2B |= (1 << CS22) | (1 << CS20); /* Prescaler 1024 */
	OCR2A = 255; /* Compare value 33ms | 30Hz */

	/* INTERRUPS INIT */
	EICRA |= (1 << ISC01); /* Falling edge of INT0 generate interrupts
							* When button is pressed generate interrupts*/
	EIMSK |= (1  << INT0); /* Activate interrupt on INT0 */

	PCICR |= (1 << PCIE2); /* Activate interrupt on PCINT0:7 and PCINT16:23 */
	PCMSK2 |= (1 << PCINT20); /* Activate interrupt on PCINT18 */
}

/* ************************************************************************** */
/*                                   LED                                      */
/* ************************************************************************** */

/**
 * @brief Light on or off the led
 *
* @param port the port register
* @param port_bit the port bit
 * @param status 1 to light on, 0 to light off
 */
static void setLedStatus(volatile uint8_t* port, uint8_t port_bit, uint8_t status) {
    if (status)
        *port |= (1 << port_bit); /* Switch On the Led
                                   * By putting a one on the port_bit position of the port
                                   */
    else
        *port &= ~(1 << port_bit); /* Switch Off the led
                                    * By putting a zero on the port_bit position of the port
                                    */
}

/**
* @brief Print the number on the leds
*
* Print the number in binary symbolsed by leds
* @param nbr the number to print
*/
static void printNbr(uint8_t nbr) {
    (void) nbr;
    setLedStatus(&PORTB, PB0, nbr & 1);
    setLedStatus(&PORTB, PB1, nbr & 2);
    setLedStatus(&PORTB, PB2, nbr & 4);
    setLedStatus(&PORTB, PB4, nbr & 8);
}

/* ************************************************************************** */
/*                               INTERRUPTS                                   */
/* ************************************************************************** */

/* Press button 1 */
ISR(INT0_vect) {
	if (!b1_debounce) {
		b1_debounce = 1;
		TCNT0 = 0; /* Reset Timer0 */
		TIMSK0 |= (1 << OCIE0A); /* Set interrupt flag */

		b1_press_dir = (EICRA & (1 << ISC00)); /* Get current press dir (pressing / releasing) */
	}
}

/* Debounce button 1 */
ISR(TIMER0_COMPA_vect) {
    static volatile uint8_t count = 0;
    count++;
    if (count >= DEBOUNCE_TIME) {

        b1_debounce = 0;
        count = 0;
        TIMSK0 &= ~(1 << OCIE0A); /* Disable interrupt */
		if (!b1_press_dir)
			nbr = (nbr + 1) % 16;
			// nbr++;
		printNbr(nbr);
		EICRA ^= (1 << ISC00); /* Toggle interrupt edge */
    }
}

/* Press button 2 */
ISR(PCINT2_vect) {
	PCICR &= ~(1 << PCIE2); /* Disable interrupt */
	_delay_ms(150);
	PCICR |= (1 << PCIE2); /* Disable interrupt */
	if (b2_last_state) {
		if (nbr == 0)
			nbr = 16;
		nbr--;
		b2_last_state = 0;
		printNbr(nbr);
		return ;
	}
	b2_last_state = 1;
}

/* Debounce button 2 */
ISR(TIMER2_COMPA_vect) {
    static volatile uint8_t count = 0;
    count++;
    if (count >= DEBOUNCE_TIME) {
		printNbr(nbr);
        b1_debounce = 0;
        count = 0;
        TIMSK2 &= ~(1 << OCIE2A); /* Disable interrupt */
		// if (!b1_press_dir)
		// 	PORTB ^= (1 << PB0); /* Toggle LED */
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
