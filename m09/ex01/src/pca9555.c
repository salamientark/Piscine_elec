/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pca9555.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:15:26 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/14 00:29:35 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

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
	if (i2c_start_mode(IO_EXPAND_ADDR, TW_WRITE))
		return (1);
	/* Send command bytes (which register to reach) */
	i2c_write(PCA9555_C0);
	status = TW_STATUS;
	if (status != TW_MT_DATA_ACK) {
		uart_printstr("Error: DATA_ACK\r\n");
		return (1);
	}
	uart_printstr(("OK1\r\n"));
	/* Change configuration register */
	i2c_write(val & 0xFF);
	i2c_write(val >> 8);

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
uint16_t	i2c_gpio_get_register_pair(const uint8_t reg) {
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

/**
 * @brief Set data for one register of the PCA9555
 *
 * @param reg -- The reister to set
 * @param val -- New register value
 */
void	i2c_gpio_set_register(const uint8_t reg, const uint8_t val) {

	i2c_start_mode(IO_EXPAND_ADDR, TW_WRITE); /* start i2c communication */
	i2c_write(reg); /* Request register */
	i2c_write(val); /* Write register */
	i2c_stop();
}

/**
 * @brief Set the PCA9555 Register pair
 *
 * First 8 bits are first are first PCA955 register
 * Last 8 bits are second PCA9555 register
 *
 * @param reg -- start register to set
 * @return uint16_t -- new register data
 */
void	i2c_gpio_set_register_pair(const uint8_t reg, const uint16_t data) {
	/* start i2c communication */
	i2c_start_mode(IO_EXPAND_ADDR, TW_WRITE);

	/* Request register */
	i2c_write(reg);

	/* Write register */
	i2c_write(data >> 8);
	i2c_write(data & 0xFF);
	i2c_stop();
}

/**
 * @brief Print number in binary on led D9, D10, D11 using the PCA955
 *
 * WARNING: YOU must ensure those pins are correctly configures
 *
 * @param nbr -- The number value
 */
void	i2c_gpio_print_nbr(const uint8_t nbr) {
	i2c_gpio_set_register(PCA9555_O0, 0xFF & ~(nbr << 1));
}
