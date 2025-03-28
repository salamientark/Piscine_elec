/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/12 21:57:39 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"


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

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();

	while (1) {
		spi_set_led(SPI_OFF, SPI_OFF, SPI_OFF);
		_delay_ms(250);
		spi_set_led(SPI_WHITE, SPI_OFF, SPI_OFF);
		_delay_ms(250);
		spi_set_led(SPI_OFF, SPI_WHITE, SPI_OFF);
		_delay_ms(250);
		spi_set_led(SPI_OFF, SPI_OFF, SPI_WHITE);
		_delay_ms(250);
	}
	
	return (0);
}
