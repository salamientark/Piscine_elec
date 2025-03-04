/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/04 20:21:42 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/**

 *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf/
 */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
// static void init(void) {
// 	TCCR1A = 0; /// Set CTC mode for timer 1
// 	TCCR1B = 0b00001101; /// Set prescaler to 1024 in CTC mode
// 	OCR1A = 15625; /// Set the timer1 speed to 1Hz
// }

/**
* @brief Initialize UART settings
*
* 1. Set baud rate
* 2. Set frame format
* 3. Enable transmitter
*/
static void uart_init(void) {
	/* Set baud rate */
	UBRR0 = 8;

	/* Set frame format: 8data bits, No parity, 1stop bit */
	UCSR0C = 0B00000110;

	/* Enable transmitter + 8data bits */
	UCSR0B |= (1 << TXEN0);
}

/**
* @brief Transmit 'Z' character to UART
*/
static void uart_tx(void) {
	while (!(UCSR0A & (1 << UDRE0))) {}

	UDR0 = 'Z';
}

int main() {
	// init();
	uart_init();
	while (1) {
		uart_tx();
		_delay_ms(1000);
	}
	return (0);
}
