/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/10 00:35:52 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <util/twi.h>

#define AHT20 0x38

/* ************************************************************************** */
/*                                    INIT                                    */
/* ************************************************************************** */


/**
 * @brief Initialize mmcu
 */
void	init(void) {
	uart_init();
	i2c_init();

	// TCCR0A |= (1 << WGM01); /* Set CTC mode */
	// TCCR0B |= (1 << CS02) | (1 << CS00); /* Set prescaler to 1024 */
	// OCR0A = 255; /* Set compare value */
	// TIMSK0 |= (1 << OCIE0A); /* Enable interrupt */
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */
int main() {
	init();

	/* AHT20 Calibratiomn */
	uint8_t	status;

	while (1) {
		/* Start request */
		i2c_start();
		status = TWSR & 0xF8; /* Get STATUS */
		if (status != TW_START) {
			uart_printstr("Error on start\r\n");
			continue ;
		}
		uart_printstr("== I2C: START msg sent\r\n");

		/* Address packet */
		i2c_write(AHT20 << 1 | TW_WRITE);
		status = TWSR & 0xF8;
		if (status != TW_MT_SLA_ACK) { /* Check aknoledgment */
			uart_printstr("Couldn't reach target on address: 0x");
			uart_printhex(AHT20);
			uart_printstr("\r\n");
			continue ;
		}
		uart_printstr("== I2C: Reached device on address: 0x");
		uart_printhex(AHT20);
		uart_printstr("\r\n");

		/* STOP request */
		i2c_stop();
		for (volatile uint16_t i = 0; i < 0xFFFF; i++) {}
	}

	return (0);
}
