/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:59:23 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/03 16:10:06 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

/**
* @brief Turn on the LED when the button is pressed
* 
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
*
*/
static void lightUp(void) {
	DDRB |= (1 << DDB0); /// Set data register to output for the led
	PORTB |= (1 << PB0); /// Set value on the PORT B
}

int main(int ac, char** av){
	(void) ac;
	(void) av;
	lightUp();
	return (0);
}
