/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pca9555.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:15:26 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/14 13:13:27 by dbaladro         ###   ########.fr       */
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
void	i2c_gpio_print_hex_nbr(const uint8_t nbr) {
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

/**
 * @brief Print one number on 7 segment display
 *
 * CA1 is the left most digit CQ4 the rightmose digit
 *
 * @param pos -- Position of the 7 segment display
 * @param val -- Value to be printed
 */
void	i2c_gpio_print_one_nbr(const uint8_t pos, uint8_t val) {
	uint16_t	i1_reg = 0;
	uint16_t	i0_reg = i2c_gpio_get_register(PCA9555_I0);

	val %= 10;
	switch (val) {
		case 0:
			i1_reg |= (1 << TOF_A) | (1 << TOF_B) | (1 << TOF_C) | (1 << TOF_D)
				| (1 << TOF_E) | (1 << TOF_F);
			break ;
		case 1:
			i1_reg |= (1 << TOF_B) | (1 << TOF_C);
			break ;
		case 2:
			i1_reg |= (1 << TOF_A) | (1 << TOF_B) | (1 << TOF_G) | (1 << TOF_E)
				| (1 << TOF_D);
			break ;
		case 3:
			i1_reg |= (1 << TOF_A) | (1 << TOF_B) | (1 << TOF_G) | (1 << TOF_C)
				| (1 << TOF_D);
			break ;
		case 4:
			i1_reg |= (1 << TOF_F) | (1 << TOF_G) | (1 << TOF_B) | (1 << TOF_C);
			break ;
		case 5:
			i1_reg |= (1 << TOF_A) | (1 << TOF_F) | (1 << TOF_G) | (1 << TOF_C)
			| (1 << TOF_D);
			break ;
		case 6:
			i1_reg |= (1 << TOF_A) | (1 << TOF_F) | (1 << TOF_G) | (1 << TOF_C)
			| (1 << TOF_D) | (1 << TOF_E);
			break ;
		case 7:
			i1_reg |= (1 << TOF_A) | (1 << TOF_B) | (1 << TOF_C);
			break ;
		case 8:
			i1_reg |= (1 << TOF_A) | (1 << TOF_B) | (1 << TOF_C) | (1 << TOF_D)
			| (1 << TOF_E) | (1 << TOF_F) | (1 << TOF_G);
			break ;
		case 9:
			i1_reg |= (1 << TOF_A) | (1 << TOF_B) | (1 << TOF_C) | (1 << TOF_D)
			| (1 << TOF_F) | (1 << TOF_G);
			break ;
	}
	/* Clear Display */
	i2c_gpio_set_register(PCA9555_O1, 0x00);

	/* Change 7 segment selectr */
	i0_reg |= ((1 << CA1) | (1 << CA2) | (1 << CA3) | (1 << CA4));
	i0_reg &= ~(1 << pos);
	i2c_gpio_set_register_pair(PCA9555_O0, (i1_reg >> 8) | (i0_reg << 8));
}

void	i2c_gpio_clear_segment(void) {
	
}

/**
 * @brief Print number on 7 segment display NON LEADING 0
 *
 * @param nbr -- nbr to print
 */
void	i2c_print_nbr(uint16_t nbr) {
	nbr %= 10000;
	i2c_gpio_print_one_nbr(CA4, nbr % 10);
	if ((nbr /= 10) == 0)
		return ;
	i2c_gpio_print_one_nbr(CA3, nbr % 10);
	if ((nbr /= 10) == 0)
		return ;
	i2c_gpio_print_one_nbr(CA2, nbr % 10);
	if ((nbr /= 10) == 0)
		return ;
	i2c_gpio_print_one_nbr(CA1, nbr % 10);
	if ((nbr /= 10) == 0)
		return ;
}
