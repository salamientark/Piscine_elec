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
 *    - Schem at Page 12
 *    - TIMER0 at Page 102-120
 *    - Interrupts at page 66 - 78
 *    - External Interrupts at Pages 79 - 84
 *    - ADC at Pages 246 - 261
 */

/* ************************************************************************** */
/*                                   INIT                                     */
/* ************************************************************************** */

/**
 * @brief Initialize UART
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
* @brief Initialize ADC registers
*/

static void	adc_init(void) {
	ADMUX = (1 << REFS0) | (1 << ADLAR); /* Set reference voltage to AVCC and select ADC0 */
	ADCSRA = 0b10000010; /* Enable ADC
						  * Set prescaler to 4 (8 bits) */
	ADCSRB = 0b00000011; /* Set Auto-trigger source to Timer0 Compare Match A */
	DIDR0 = 0b00111000; /* Enable digital input on ADC0, ADC1, ADC2 */
}

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	uart_init();
	adc_init();
}

/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */

/**
 * @brief Transmit a character to UART
 */
static void uart_tx(char c) {
	while (!(UCSR0A & (1 << UDRE0))) {} /* Check if the transmit buffer is empty */
	UDR0 = c; /* Write the character to the USART data register */
}

/**
 * @brief Send string to UART transmitter
 *
 * @param str - string to send
 */
static void uart_printstr(const char* str) {
	while (*str) {
		uart_tx(*str);
		str++;
	}
}

/**
 * @brief Send 8 bit hex value to UART Tx
 *
 * @param c - hex value to send
 */
static void uart_printhex(unsigned char c) {
	uint8_t val = c >> 4;
	if (val < 10)
		val += '0';
	else
		val += 'a' - 10;
	uart_tx(val);
	val = c & 0xF;
	if (val < 10)
		val += '0';
	else
		val += 'a' - 10;
	uart_tx(val);
}

/* ************************************************************************** */
/*                                    ADC                                     */
/* ************************************************************************** */

/**
* @brief Read ADC value
*
* @param ch - channel to read
*
* @return ADC value
*/
static uint8_t	adc_read(uint8_t ch) {
	ADCSRA |= (1 << ADSC); /* Start conversation */
	ch &= 0b00000111; /* Protection sclaing ch to 0:7 
					   * ch 8 is internal tem sensor
					   */
	ADMUX = (ADMUX & 0b11111000) | ch; /* Enable only active channel */
	while (ADCSRA & (1 << ADSC)) {} /* Wait until ADC conversation complete */
	return (ADCH);
}

/* ************************************************************************** */
/*                               INTERRUPTS                                   */
/* ************************************************************************** */

/* Read ADC val */
ISR(TIMER0_COMPA_vect) {
	/* READ LDR Value */
	uint8_t adc = adc_read(1);
	uart_printhex(adc);
	uart_printstr(", ");

	/* READ Temp */
	adc = adc_read(2);
	uart_printhex(adc);
	uart_printstr(", ");

	/* READ Pot value */
	adc = adc_read(0);
	uart_printhex(adc);

	uart_printstr("\n\r");
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();
	sei(); /* Enable global interrupts */

	uint8_t	adc = 0;
	while (1) {
		adc = adc_read(1);
		uart_printhex(adc);
		uart_printstr(", ");
		adc = adc_read(2);
		uart_printhex(adc);
		uart_printstr(", ");
		adc = adc_read(0);
		uart_printhex(adc);

		uart_printstr("\n\r");
		_delay_ms(20);
	}

	return (0);
}
