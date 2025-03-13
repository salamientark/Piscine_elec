/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 19:30:55 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <stdint.h>
#include <util/delay.h>
#include <util/twi.h>

#define IO_EXPAND_ADDR 0x20

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

/* PCA9555 ALIASES */
#define PCA9555_I0 0x00 /* INPUT PORT 0 */
#define PCA9555_I1 0x01 /* INPUT PORT 1 */
#define PCA9555_O0 0x02 /* OUTPUT PORT 0 */
#define PCA9555_O1 0x03 /* OUTPUT PORT 1 */
#define PCA9555_P0 0x04 /* POLARITY INVERSION PORT 0 */
#define PCA9555_P1 0x05 /* POLARITY INVERSION PORT 0 */
#define PCA9555_C0 0x06 /* CONFIGURATION PORT 0 */
#define PCA9555_C1 0x07 /* CONFIGURATION PORT 1 */
#define PCA9555_INPUT 0
#define PCA9555_OUTPUT 1

/* EXPANDER PIN */
#define SW3		0x00
#define D11		0x01
#define D10		0x02
#define D9		0x03

/**
 * @brief Set the gpio expander io function
 *
 * send 16 bits fort 16pins. 0: INPUT | 1: OUTPUT
 *
 * @param val -- GPIO val
 * @return 0 on success, 1 on error
 */
uint8_t	i2c_gpio_setio(const uint16_t val) {
	uint8_t	status = 0;

	/* start i2c communication */
	i2c_start_mode(IO_EXPAND_ADDR, TW_WRITE);
	status = TW_STATUS;
	if (status != TW_START && status != TW_REP_START)
		return (1);

	/* Send command bytes (which register to reach) */
	i2c_write(PCA9555_C0);
	/* Change configuration register */
	i2c_write(val >> 8);
	i2c_write(val & 0xFF);

	/* Stop i2c communication */
	i2c_stop();

	return (0);
}

/**
 * @brief Return the PCA9555 Register pair
 *
 * First 8 bits are first are first PCA955 register
 * Last 8 bits are second PCA9555 register
 *
 * @param reg -- Register to reach
 * @return uint16_t -- Register pair as one bit
 */
uint16_t	i2c_gpio_get_register(const uint8_t reg) {
	uint16_t	res = 0;

	/* start i2c communication */
	i2c_start_mode(IO_EXPAND_ADDR, TW_WRITE);

	/* Request register */
	i2c_write(reg);

	/* Read Response */
	i2c_start_mode(IO_EXPAND_ADDR, TW_READ);
	res |= i2c_read_ack() << 8;
	res |= i2c_read_nack();
	i2c_stop();

	return (res);
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	uint16_t	reg = 0;
	init();


	/* Get config register */
	reg = i2c_gpio_get_register(PCA9555_C0);
	uart_printstr("Register: ");
	uart_printhex(reg >> 8);
	uart_printhex(reg & 0xFF);
	uart_printstr("\r\n");

	while (1) {
		// ft_memset(buff, 0x00, 8);

		// i2c_gpio_get_register(0x)
		_delay_ms(1000);
	}
	
	return (0);
}
