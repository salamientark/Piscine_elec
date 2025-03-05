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

/**

 *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf/
 *  CORE SREG register description page 20
*  INTERRUPTS description at pages 74
 *  TIMER0 settings at pages 102-119
 *  USART seeting are described at pages 179-204
 */

/* ************************************************************************** */
/*                                   INIT                                     */
/* ************************************************************************** */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	/* TIMER10 Setup */
	// TCCR1A = 0b00000000; /* Set CTC mode */
	// TCCR1B = 0b00001101; /* Set prescaler to 1024 */
	// OCR1A = (uint16_t)2 * F_CPU / 1024 - 1; /* Set compare value */
	// TIMSK1 = 0b00000010; /* Enable interrupt when comapre with OCCR0A occur */
}

/**
* @brief Initialize UART settings
*
* 1. Set baud rate
* 2. Set frame format
* 3. Enable transmitter
*/
static void uart_init(void) {
	/* Set baud rate */
	UBRR0 = F_CPU / (16 * (UART_BAUDRATE + 1)); /* Set Baud rate in the
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
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); /* Enable transmitter */
}


/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */

/**
 * @brief Transmit 'Z' character to UART
 *
* @param c - character to send
 */
static void uart_tx(char c) {
	while (!(UCSR0A & (1 << UDRE0))) {} /* Check if the transmit buffer is empty */
	UDR0 = c; /* Write the character to the USART data register */
}

/**
* @brief Receive character from UART
*
* @return character that was read
*/
static char uart_rx(void) {
	while (!(UCSR0A & (1 << RXC0))) {} /* Check if the receive buffer is empty */
	return (UDR0); /* Send back the character that was read */
}

/**
 * @brief Send string to UART transmitter
 *
 * @param str - string to send
 */
// static void uart_printstr(const char* str) {
// 	while (*str) {
// 		uart_tx(*str);
// 		str++;
// 	}
// }

/* ************************************************************************** */
/*                                 INTERUPTS                                  */
/* ************************************************************************** */

ISR(USART_RX_vect) {
	/* This function is called when the TIMER0_COMPARE_A interrupts occurs */
	uart_tx(uart_rx());
}


/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */
int main() {
	init();
	uart_init();
	sei(); /* Enable global interrupts
			* Same as SREG |= (1 << 7);
			*/
	while (1) {}
	return (0);
}
