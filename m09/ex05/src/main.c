/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/14 11:46:48 by dbaladro         ###   ########.fr       */
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

uint16_t g_nbr = 0;

/* ************************************************************************** */
/*                                    INIT                                    */
/* ************************************************************************** */

/**
 * @brief Initialize mmcu
 */
void	init(void) {
	i2c_init();
	uart_init();
	/* Set PCA9555 Pin */
	i2c_gpio_reset_default_register();
	i2c_gpio_setio(0xFFFF & ~((1 << D9) | (1 << D10) | (1 << D11)
				| (1 << CA1) | (1 << CA2) | (1 << CA3) | (1 << CA4)
				| (1 <<  TOF_A) | (1 << TOF_B) | (1 << TOF_C) | (1 << TOF_D)
				| (1 << TOF_E) | (1 << TOF_F) | (1 << TOF_G) | (1 << TOF_DPX))); /* | (1 << SW3) */

	/* Init timer to 1Hz */
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // 1024 prescaler
	OCR1A = F_CPU / 1024; // 30hz
	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */
ISR(TIMER1_COMPA_vect) {
	g_nbr = (g_nbr + 1) % 10000;
}


int main() {

	init();
	SREG |= (1 << SREG_I); // Enable global interrupt
	while (1)
	{
		i2c_print_nbr(g_nbr);
	}
	
	return (0);
}
