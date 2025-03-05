/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:59:23 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/03 16:10:47 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

/**
* The DDRB register defines the DATA_DIRECTION_REGISTER B
* DDB0 is the first bit of the DDRB register
*
* PORTB define the PORT_REGISTER B
* PB0 is the first bit of PORTB
*
* (1 << DDB0) is a bit shift to the left of 1 by DDB0 pos
* |= is an OR operation and assignation
*
* So we set the data direction bit to 1 (Output / write)
* Then we write the data on the port to 1 (High)
*
* All the information that I needed for this can be found at:
*  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf/
*  Configuring the PIN and data_direction page 85-86
*  Register description can be found at pages 100-101
*/

/**
 * @brief Init the direction data registers
 */
static void init(void) {
	// DDRB = 0xFF;
	// DDRD = 0xFF;

	DDRB |= (1 << DDB0); /// Set data register to output for the led
	DDRD &= ~(1 << DDD2); /// Set the data direction to input for the button
}

/**
 * @brief Return led status
 *
 */
static uint8_t getLedStatus(void) {
	return (PORTB & (1 << PB0)); /// Make a mask getting only the first bit of PORTB
}

/**
 * @brief Read button status
 *
 * @return 1 if button is pressed, else 0
 */
static uint8_t pressedButton(void) {
	return (PIND & (1<< PD2)); /// Make a mask getting only the second bit of PIND
}

/**
 * @brief Light on or off the led
 *
* @param status 1 to light on, 0 to light off
*/
static void setLedStatus(uint8_t status) {
	if (status)
		PORTB |= (1 << PB0); /// Switch On the Led
	else
		PORTB &= ~(1 << PB0); /// Switch Off the led
}


int main(int ac, char** av){
	(void) ac;
	(void) av;

	init();
	uint8_t status = 0;
	while (1) {
		status = pressedButton();
		if (status == 0)
			setLedStatus(getLedStatus() ^ 1);
		while (status == 0)
			status = pressedButton();
		_delay_ms(100);
	}
	return (0);
}
