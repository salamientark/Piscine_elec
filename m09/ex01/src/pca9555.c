/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pca9555.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:15:26 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/14 09:22:51 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * PCA9555 lib 
 */

/**
 * @brief Reset PCA9555 default values according to datasheet
 */
void	i2c_gpio_reset_default_register(void) {
	i2c_start_mode(PCA9555_ADDR, TW_WRITE);
	i2c_write(PCA9555_O0); /* OUPUT PORT REGISTER START */
	i2c_write(0xFF); /* OUTPUT PORT CONTROL 0 */
	i2c_write(0xFF); /* OUTPUT PORT CONTROL 1 */
	i2c_write(0x00); /* POLARITY INVERSION 0 */
	i2c_write(0x00); /* POLARITY INVERSION 1 */
	i2c_write(0xFF); /* CONFIGURATION 0 */
	i2c_write(0xFF); /* CONFIGURATION 1 */
	i2c_stop();
}

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
	if (i2c_start_mode(PCA9555_ADDR, TW_WRITE))
		return (1);
	/* Send command bytes (which register to reach) */
	i2c_write(PCA9555_C0);
	status = TW_STATUS;
	if (status != TW_MT_DATA_ACK) {
		uart_printstr("Error: DATA_ACK\r\n");
		return (1);
	}
	/* Change configuration register */
	i2c_write(val & 0xFF);
	i2c_write(val >> 8);

	/* Stop i2c communication */
	i2c_stop();

	return (0);
}

/**
 * @brief Return the PCA9555 Register value
 *
 * @param reg -- Register to reach
 * @return uint8_t -- Register value
 */
uint8_t	i2c_gpio_get_register(const uint8_t reg) {
	uint8_t	val = 0;

	/* start i2c communication */
	i2c_start_mode(PCA9555_ADDR, TW_WRITE);

	/* Request register */
	i2c_write(reg);

	/* Read Response */
	i2c_start_mode(PCA9555_ADDR, TW_READ);
	val = i2c_read_nack();
	i2c_stop();

	return (val);
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
	i2c_start_mode(PCA9555_ADDR, TW_WRITE);

	/* Request register */
	i2c_write(reg);

	/* Read Response */
	i2c_start_mode(PCA9555_ADDR, TW_READ);
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

	i2c_start_mode(PCA9555_ADDR, TW_WRITE); /* start i2c communication */
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
	i2c_start_mode(PCA9555_ADDR, TW_WRITE);

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

/**
 * @brief Get button state using the PCA9555
 *
 * WARNING: YOU must ensure the button pin is correctly configured as input
 *
 * @return uint8_t -- 1 when release, 0 when pressed
 */
uint8_t	i2c_gpio_get_button_state(void) {
	return (i2c_gpio_get_register(PCA9555_I0) & 0x01);
}
