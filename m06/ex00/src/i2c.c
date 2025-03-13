/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:27:28 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/09 23:43:47 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Init I2C interface
 */
void i2c_init(void) {
	/* Set SCL to 100Khhz */
	TWBR = 1; /* Clock division factor */
	TWSR |= (1 << TWPS1) | (1 << TWPS0); /* Set prescaler to 1024 */
}

/**
 * @brief Start I2C communication
 */
void i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); /* Enable TWI
													   * Clear TWINT flag
													   * generate start condition */
	while(!(TWCR & (1 << TWINT))) {} /* Wait for TWI flag set */
}

/**
 * @brief Stop I2C communication (realease bus)
 */
void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); /* Enable TWI,
													   * Clear TWINT flag
													   * generate stop condition */
}

/**
* @brief Send data package
 *
 * @param address -- Slave address
 * @param mode -- 0 for write, 1 for read
 */
void	i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); /* Enable TWI,
 										* Clear TWINT flag */
	while(!(TWCR & (1 << TWINT))) {} /* Wait for TWI flag set */
}

/**
 * @brief Read data package with ACK
 *
 * @return received data
 */
uint8_t	i2c_read_ack(void) {
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); /* Enable TWI,
													  * Clear TWINT flag
													  * Enable ACK */
	while (!(TWCR & (1 << TWINT))) {} /* Wait for TWI flag set */
	return (TWDR); /* Return received data */
}

/**
 * @brief Read data package with NO ACK
 *
 * @return received data
 */
uint8_t	i2c_read_nack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN); /* Enable TWI,
									    * Clear TWINT flag
									    * Disable ACK */
	while (!(TWCR & (1 << TWINT))) {} /* Wait for TWI flag set */
	return (TWDR); /* Return received data */
}
