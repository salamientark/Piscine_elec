/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:35:51 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 00:47:28 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Initialize UART
 */
void uart_init(void) {
	/* Set baud rate */
	UBRR0 = 8;
	/* UBRR0 = round((double)F_CPU / 16 / UART_BAUDRATE - 1); Set Baud rate in the
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
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); /* Enable transmitter */
}

/**
 * @brief Receive a character to UART
 *
 * @param c -- The character to transmit
 */
uint8_t uart_rx(void) {
	while (!(UCSR0A & (1 << RXC0))) {} /* Wait until data is received */
	return UDR0; /* Get and return received data from buffer */
}

/**
 * @brief Transmit a character to UART
 */
void uart_tx(const char c) {
	while (!(UCSR0A & (1 << UDRE0))) {} /* Check if the transmit buffer is empty */
	UDR0 = c; /* Write the character to the USART data register */
}

/**
 * @brief Send string to UART transmitter
 *
 * @param str - string to send
 */
void uart_printstr(const char* str) {
	while (*str) {
		uart_tx(*str);
		str++;
	}
}
/**
 * @brief Print hex value to UART
 *
 * @param c -- char
 */
void	uart_printhex(unsigned char c) {
	uart_tx((c >> 4) < 10 ? (c >> 4) + '0' : (c >> 4) + 'a' - 10);
	uart_tx((c & 0xF) < 10 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10);
}

/**
 * @brief Print decimal value to UART
 *
 * @param nbr -- number to print
 */
void	uart_printdec(unsigned int nbr) {
	if (nbr > 9)
		uart_printdec(nbr / 10);
	uart_tx("0123456789"[nbr % 10]);
}

/**
 * @brief Read input from UART
 *
 * @param buffer -- buffer to store input
 * @param size -- buffer size
* @return uint8_t -- number of characters read
 */
uint8_t uart_read_input(unsigned char* buffer, const uint8_t size) {
    unsigned char    c;
    uint8_t index = 0;

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
			if (index == 0)
				continue ;
            uart_printstr("\r\n");
            break ;
        }
        if (index == size - 1) /* Buffer end reached */
            continue ;
        if (c < ' ' || c > 127) /* Non printable char */
            continue ;
        buffer[index] = c;
		uart_tx(c);
        index++;
    }
	return (index);
}
