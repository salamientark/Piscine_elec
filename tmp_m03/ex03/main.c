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
 */

/* ************************************************************************** */
/*                                   INIT                                     */
/* ************************************************************************** */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	/* RGB LED INIT */
	DDRD = 0b01101000; /// Set RGB LED as outputs in the DATA_DIRECTION_REGISTER
	
	/* SET RGB LED TO OFF BY DEFAULT */

	/* Default OFF value is 0xFF because of inverting mode */
	OCR0A = 0xFF; /* Set GREEN part to off */
	OCR0B = 0xFF; /* Set RED part to off */
	OCR2B = 0xFF; /* Set BLUE part to off */
}

/**
 * @brief Initialize UART communication
 */
static void init_uart(void) {
	/* Set baud rate */
	UBRR0 = round((double)F_CPU / 16 / UART_BAUDRATE - 1);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = 0b00000110; /* Asynchrom=nous mode; 8bit word; no parity; 1 stop bit */
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

/**
* @brief Init timers for RGB LED
*/
static void init_rgb(void) {
	/* TIMER0 Setup TO Hz */
	TCCR0A = 0b11110011; /* OC0A and OC0B in Inverting mode
						  * and set Mode To Fast PWM with TOP = 0xFF
						  */
	TCCR0B = 0b00000001; /* Set no prescaler */

	/* TIMER2 Setup TO Hz */
	TCCR2A = 0b00110011; /* OC2A in Inverting mode
						  * and set Mode To Fast PWM with TOP = 0xFF
						  */
	TCCR2B = 0b00000001; /* Set prescaler no prescaler */
}

/* ************************************************************************** */
/*                                    UART                                    */
/* ************************************************************************** */

/**
 * @brief Transmit a character from UART
 *
 * @return char -- The received character
 */
static void uart_tx(char c) {
	while (!(UCSR0A & (1 << UDRE0))) {} /* Wait until UDR0 is ready to receive data */
	UDR0 = c; /* Send data */
}

/**
 * @brief Receive a character to UART
 *
 * @param c -- The character to transmit
 */
static uint8_t uart_rx(void) {
	while (!(UCSR0A & (1 << RXC0))) {} /* Wait until data is received */
	return UDR0; /* Get and return received data from buffer */
}

/**
 * @brief Transmit a string from UART
 *
 * @param str -- The string to transmit
 */
static void uart_printstr(const char* str) {
	while (*str)
		uart_tx(*str++);
}

/**
 * @brief Receive a string from UART
 *
 * @param buffer -- The buffer to store the received string
 * @param size -- The size of the buffer
 */
static void uart_get_input(char* buffer, const uint8_t size) {
	char	c;
	uint8_t	index = 0;

	while (1) {
		c = uart_rx();
		if (c == 0x7F) { /* DEL */
			if (index > 0) {
				index--;
				buffer[index] = 0;
				uart_printstr("\b \b");
			}
			continue ;
		}
		if (c == '\r') { /* Enter pressed */
			uart_printstr("\r\n");
			break ;
		}
		if (index == size - 1) /* Buffer end reached */
			continue ;
		if (c < ' ') /* Non printable char */
			continue ;
		buffer[index] = c;
		uart_tx(c);
		index++;
	}
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
static void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0B = ~r; /* Value are inverted because of inverting mode */
	OCR0A = ~g;
	OCR2B = ~b;
}

/**
 * @brief Change RGB LED color based on RGB hex value
 *
 * @param buffer -- RGB HEX value
 * @param size -- buffer size
 */
static void change_color(const char* buffer, const uint8_t size) {
	uint8_t			index = 0;
	uint64_t		color = 0;
	unsigned char	c;

	if (buffer[size - 1] != 0)
		return ; /* Invalid Hex value */
	if (buffer[index++] != '#') /* Invalid format */
		return ;

	while (index < 7) {
		color <<= 8;
		c = buffer[index];
		if ('0' <= c && c <= '9')
			color |= (c - '0');
		else if ('a' <= c && c <= 'f')
			color |= (c - 'a' + 10);
		else if ('A' <= c && c <= 'F')
			color |= (c - 'A' + 10);
		else
			return ; /* Invalid hex number */
		index++;
	}
	set_rgb(color >> 32, color >> 16 & 0xFFFF, color & 0xFFFF);
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();
	init_uart();
	init_rgb();

	char	input[8] = {0};
	while (1) {
		uart_get_input(input, 8);
		change_color(input, 8);
		for (uint8_t i = 0; i < 8; i++)
			input[i] = 0;
	}
	return (0);
}
