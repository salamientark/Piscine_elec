/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/04 10:54:16 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/**
 * Volatile variables are variables that may changed at any moment.
 * Like at interuption pin signal... So the compiler will not try to optimize it
 * This is particularly usefull when you want to be sure a variable will not 
 * change during compilation or else

 *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf/
 *  Configuring the PIN Page 12
 *  And Timer1 register datas are found from page 120-145
 *  Register description can be found at pages 100-101
 */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	DDRB |= (1 << PB1); /* Set PB1 (LED1) pin as output */
	TCCR1A = 0B01000000; /* Toggle OC1A on compare match 
						  * and set Waveform Generation to CTC
						  * Make the led 1 blink
						  */
	TCCR1B = 0B00001101; /* Set Wave Generation Mode to CTC
						 * Set prescaler to 1024
						 */
	OCR1AH = 0x1E; /* Set the high byte of the output compare register */
	OCR1AL = 0x84; /* Set the low byte of the output compare register */
}


int main() {
	init();
	while (1) {}
}
