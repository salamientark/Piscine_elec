/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:07:31 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/12 20:08:52 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
* @brief Initialize ADC registers
*/
void	adc_init(void) {
	ADMUX = (1 << REFS0) | (1 << ADLAR); /* Set reference voltage to AVCC and select ADC0 */
	ADCSRA = 0b10000010; /* Enable ADC
						  * Set prescaler to 4 (8 bits) */
	DIDR0 = 0b00111110; /* Enable digital input on ADC0 */
}

/**
* @brief Read ADC value
*
* @param ch - channel to read
*
* @return ADC value
*/
uint8_t	adc_read(uint8_t ch) {
	ch &= 0b00000111; /* Protection sclaing ch to 0:7 
					   * ch 8 is internal tem sensor
					   */
	ADMUX = (ADMUX & 0b11111000) | ch; /* Enable only active channel */
	ADCSRA |= (1 << ADSC); /* Start conversation */
	while (ADCSRA & (1 << ADSC)) {} /* Wait until ADC conversation complete */
	return (ADCH);
}
