/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/14 00:31:02 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <stdint.h>


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
	i2c_init();
	uart_init();
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

// void	i2c_gpio_invert_pin_polarity(const uint16_t pin) {
// 	uint16_t	pol = i2c_gpio_get_register_pair(PCA9555_P0);
// 	pol ^= (1 << pin); /* Inverst polarity on pin */
// 	i2c_gpio_set_register_pair(PCA9555_P0, pol);
// }

int main() {
	init();

	/* Set D9, D10, D11 as output */
	i2c_gpio_setio(0xFFFF);
	i2c_gpio_setio(0xFFFF & ~((1 << D9) | (1 << D10) | (1 << D11)));

	uint8_t nbr = 0;
	while (1) {
		i2c_gpio_print_nbr(nbr);
		nbr = (nbr + 1) % 8;
		_delay_ms(1000);
	}
	
	return (0);
}
