/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/05 19:18:42 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/**

 *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf/
 *  USART seeting are described at pages 179-204
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
	UBRR0 = round((double)F_CPU / 16 / UART_BAUDRATE - 1); /* Set Baud rate in the
												        * USART Baud Rate registers
												        */

	/* Set frame format: 8data bits, No parity, 1stop bit */
	UCSR0C = 0B00000110; /* Set mode to:
						  * Asynchronous USART
						  * No Parity
						  * 1 Stop bit
						  * 8-bit word size
						  */

	/* Enable transmitter + 8data bits */
	UCSR0B |= (1 << TXEN0); /* Enable transmitter */
}

/**
* @brief Transmit a character to UART
*/
static void uart_tx(char c) {
	while (!(UCSR0A & (1 << UDRE0))) {} /* Check if the transmit buffer is empty */

	UDR0 = c; /* Write the character to the USART data register */
}

int main() {
	uart_init();
	while (1) {
		uart_tx('Z');
		_delay_ms(1000);
	}
	return (0);
}
