/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */ /*   main.c                                             :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
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

void i2c_init(void) {
	/* Set SCL to 100Khhz */
}

/**
 * @brief Initialize DATA_DIRECTION registers
 */
 void init(void) {
	/* D1..4 LED as output */
	DDRB = 0b00010111;

	/* RGB LED as output */
	DDRD = 0b01101000; /// Set RGB LED as outputs in the DATA_DIRECTION_REGISTER

}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();
	sei(); /* Enable global interrupts */

	while (1) {
	}

	return (0);
}
