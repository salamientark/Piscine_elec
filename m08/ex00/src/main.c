/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/12 19:39:22 by dbaladro         ###   ########.fr       */
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

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();

	spi_set_led(0x1FFF0000, 0x00, 0x00);

	while (1) {}
	
	return (0);
}
