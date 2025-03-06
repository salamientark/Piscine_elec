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

#define USERNAME "dbaladro"
#define PASSWORD "password"

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
	
	/* LED INIT */
	DDRB = 0b00010111; // Set LED as outputs in the DATA_DIRECTION_REGISTER */
	PORTB &= ~((1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0)); /* Set LEDS off by default */
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
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); /* Enable transmitter
															* Enable Receiver
															* Add receive complete interupts
															*/
}


/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */

/**
 * @brief Transmit a character to UART
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
static void uart_printstr(const char* str) {
	while (*str) {
		uart_tx(*str);
		str++;
	}
}

/* ************************************************************************** */
/*                                 INTERUPTS                                  */
/* ************************************************************************** */


/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

/**
 * @brief Read user input from UART until enter is pressed
 *
 * @param buffer - buffer to store the input
 * @param size - size of the buffer
 * @param clear_mode - mode of the input (1: Clear text / 0: Password)
 *
 * @return pointer to the buffer
 */
static void read_user_input(char* buffer, uint8_t size, uint8_t clear_mode) {
	char	c;
	uint8_t	index = 0;

	while (1) {
		c = uart_rx();
		if (c == 0x7F) { // Check for backspace character
			if (index != 0) {
				index--;
				buffer[index] = '\0';
				uart_printstr("\b \b");
			}
			continue ;
		}
		if (c == '\r') {
			uart_printstr("\n\r");
			break ;
		}
		if (index == size - 1)
			continue ;
		buffer[index] = c;
		clear_mode == 1 ? uart_tx(c) : uart_tx('*');
		index++;
	}
}

/**
 * @brief Compare the credentials with the stored ones
 *
 * @param name - username to compare
 * @param pass - password to compare
 *
 * @return 1 if the credentials are correct, 0 otherwise
 */
static uint8_t compare_credentials(const char* name, const char* pass) {
	uint8_t idx = 0;

	while (idx < 100 && name[idx]) {
		if (name[idx] != USERNAME[idx])
			return (0);
		idx++;
	}
	if (idx < 100 && name[idx] != USERNAME[idx])
		return (0);
	idx = 0;
	while (idx < sizeof(pass) && pass[idx]) {
		if (pass[idx] != PASSWORD[idx])
			return (0);
		idx++;
	}
	return (1);
}

/**
 * @brief Inifinite loop until the user login
 */
static void login(void) {
	char		name_input[100] = {0};
	char		pass_input[100] = {0};
	while (1) {
		uart_printstr("Enter your login:\n\r");
		uart_printstr("\tUsername: ");
		read_user_input(name_input, 100, 1);
		uart_printstr("\tPassword: ");
		read_user_input(pass_input, 100, 0);
		if (compare_credentials(name_input, pass_input))
			break ;
		uart_printstr("Bad combination username/password\n\n\r");
		for (uint8_t i = 0; i < 100; i++) {
			name_input[i] = '\0';
			pass_input[i] = '\0';
		}
	}

	uart_printstr("Hello ");
	uart_printstr(name_input);
	uart_printstr("!\n\r");
	uart_printstr("Shall we play a game?\r\n");
}

/**
 * @brief Dramatic effect !!!
 */
static void	dramatic_effect(void) {
	PORTB ^= (1 << PB0); /* LED 0 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB0); /* LED 0 OFF */
	PORTB ^= (1 << PB1); /* LED 1 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB1); /* LED 1 OFF */
	PORTB ^= (1 << PB2); /* LED 2 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB2); /* LED 2 OFF */
	PORTB ^= (1 << PB4); /* LED 4 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB4); /* LED 4 OFF */
	// PORTB ^= (1 << PB4); /* LED 4 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB4); /* LED 4 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB4); /* LED 4 OFF */
	PORTB ^= (1 << PB2); /* LED 2 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB2); /* LED 2 OFF */
	PORTB ^= (1 << PB1); /* LED 1 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB1); /* LED 1 OFF */
	PORTB ^= (1 << PB0); /* LED 0 ON */
	_delay_ms(200);
	PORTB ^= (1 << PB0); /* LED 0 OFF */
	_delay_ms(200);

}

int main() {
	init();
	uart_init();

	login();
	while (1) {
		dramatic_effect();
	}
	return (0);
}
