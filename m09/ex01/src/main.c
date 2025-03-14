/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/14 13:40:34 by dbaladro         ###   ########.fr       */
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
	i2c_init();
	uart_init();
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */
int main() {
	init();

	/* Set D9, D10, D11 as output */
	i2c_gpio_setio(0xFFFF);
	i2c_gpio_setio(0xFFFF & ~((1 << D9) | (1 << D10) | (1 << D11))); /* | (1 << SW3) */

	uint8_t	i2c_button_last_state = 0;
	uint8_t	i2c_button_state = 0;

	uint8_t nbr = 0;
	while (1) {
		i2c_button_state = i2c_gpio_get_button_state();
		if (i2c_button_state != i2c_button_last_state) {
			if (i2c_button_state == 0)
				nbr = (nbr + 1) % 8;
			i2c_gpio_print_nbr(nbr);
		}
		i2c_button_last_state = i2c_button_state;
		_delay_ms(20);
	}
	
	return (0);
}
