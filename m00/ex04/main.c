/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:59:23 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/03 16:10:43 by dbaladro         ###   ########.fr       */
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
	DDRB |= (1 << DDB0); /// Set data register to output for the led
	DDRB |= (1 << DDB1); /// Set data register to output for the led
	DDRB |= (1 << DDB2); /// Set data register to output for the led
	DDRB |= (1 << DDB4); /// Set data register to output for the led

	DDRD &= ~(1 << DDD2); /// Set the data direction to input for the button
	DDRD &= ~(1 << DDD4); /// Set the data direction to input for the button
}


/**
 * @brief Read button status
 *
* @param pin_x the pin register
* @param pin_x_bit the pin bit
* @return 1 if button is pressed, else 0
*/
static uint8_t getButtonStatus(uint8_t pin_x, uint8_t port_bit) {
	return (pin_x & (1 << port_bit)); /* Read the port_bit position of the pin_x register
										*/
}

/**
 * @brief Light on or off the led
 *
* @param port the port register
* @param port_bit the port bit
 * @param status 1 to light on, 0 to light off
 */
static void setLedStatus(volatile uint8_t* port, uint8_t port_bit, uint8_t status) {
	if (status)
		*port |= (1 << port_bit); /* Switch On the Led
								   * By putting a one on the port_bit position of the port
								   */
	else
		*port &= ~(1 << port_bit); /* Switch Off the led
									* By putting a zero on the port_bit position of the port
									*/
}

/**
* @brief Print the number on the leds
*
* Print the number in binary symbolsed by leds
* @param nbr the number to print
*/
static void printNbr(uint8_t nbr) {
	(void) nbr;
	setLedStatus(&PORTB, PB0, nbr & 1);
	setLedStatus(&PORTB, PB1, nbr & 2);
	setLedStatus(&PORTB, PB2, nbr & 4);
	setLedStatus(&PORTB, PB4, nbr & 8);
}


int main(int ac, char** av){
	(void) ac;
	(void) av;

	init();
	uint8_t b1_last_state = 1;
	uint8_t b2_last_state = 1;
	uint8_t b1_status;
	uint8_t b2_status;
	uint8_t nbr = 1;
	while (1) {
		b1_status = getButtonStatus(PIND, PD2);
		b2_status = getButtonStatus(PIND, PD4);

		if (b1_status == 0 && b1_last_state == 1) /// Pressed button
			b1_last_state = 0;
		if (b2_status == 0 && b2_last_state == 1) /// Pressed button
			b2_last_state = 0;
		if (b1_status && b1_last_state == 0) { /// Released button
			nbr = (nbr + 1) % 16;
			b1_last_state = 1;
		}
		if (b2_status && b2_last_state == 0) { /// Released button}
			if (nbr == 0)
				nbr = 15;
			else
				nbr--;
			b2_last_state = 1;
		}
		printNbr(nbr);
		_delay_ms(10);
	}
	return (0);
}
