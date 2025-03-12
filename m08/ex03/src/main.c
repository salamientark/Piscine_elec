/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/12 21:50:54 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <stdint.h>
#include <util/delay.h>


/** *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 *    - Schem at Page 12
 *    - TIMER0 at Page 102-120
 *    - Interrupts at page 66 - 78
 *    - External Interrupts at Pages 79 - 84
 */

/* ************************************************************************** */
/*                                    INIT                                    */
/* ************************************************************************** */

/**
 * @brief Initialize mmcu
 */
void	init(void) {
	uart_init();
	adc_init();
	spi_init_master();
}

/* ************************************************************************** */
/*                                    TEST                                    */
/* ************************************************************************** */
// void	print_buffer(const unsigned char *buff, const unsigned int size) {
// 	for (unsigned int i = 0; i < size; i++) {
// 		if (i % 2 == 0)
// 			uart_tx(' ');
// 		uart_printhex(buff[i]);
// 	}
// 	uart_printstr("\r\n");
// }
//
/**
 * @brief Set LED color and brightness based on adc value
 *
 * @parma adc -- readed adc value (8 bit)
 */
void	set_adc_color(const uint8_t adc) {
	unsigned int	b1 = (adc < STEP ? adc : STEP) / STEP * 31.;
	unsigned int	b2 = adc > STEP ? (adc < 2 * STEP ? (adc - STEP) / STEP * 31 : 31) : 0;
	unsigned int	b3 = adc > 2 * STEP ? (adc - 2 * STEP) / (255. - 2 * STEP) * 31 : 0;

	/* Set LED COLOR */
	spi_set_led((0xE0 | ((uint32_t)(uint8_t)b1 & 0x1F)) << 24 | SPI_RED,
				(0xE0 | ((uint32_t)(uint8_t)b2 & 0x1F)) << 24 | SPI_YELLOW,
				(0xE0 | ((uint32_t)(uint8_t)b3 & 0x1F)) << 24 | SPI_GREEN);
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	uint8_t	adc_val;

	init();
	while (1) {
		adc_val = adc_read(0);
		set_adc_color(adc_val);
	}
	
	return (0);
}
