/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:27:28 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 10:38:05 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <stdint.h>

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

/**
* @brief Start I2C conversation
*
* Send START messge and Set mode
*
* @param slave -- Slave address
* @param mode -- Read or Write mode
* @return 0 on success, else 0
*/
uint8_t i2c_start_mode(uint8_t slave, uint8_t mode) {
	if (mode != TW_READ && mode != TW_WRITE)
		return (0);
	uint8_t	status = 0;
	i2c_start();
	status = TWSR & 0xF8; /* Get STATUS */
	if (status != TW_START && status != TW_REP_START) {
		uart_printstr(" == I2C: ERROR: expected TW_START or TW_REP_START\r\n");
		return (1);
	}
	i2c_write(slave << 1 | mode);
	status = TWSR & 0xF8;
	if ((mode == TW_READ && status != TW_MR_SLA_ACK)
			|| (mode == TW_WRITE && status != TW_MT_SLA_ACK)) {
		uart_printstr("Couldn't reach target on address: 0x");
		print_hex_value(slave);
		uart_printstr("\r\n");
		i2c_stop();
		return (1);
	}
	return (0);
}

/**
 * @brief Send up to 255 bits of data to slave
 *
 * @param buffer -- Data to send
 * @size -- buffer size
 * @slave -- Slave address
* @return number of bytes sent
 */
uint8_t	i2c_send_str(const char* buffer, uint8_t size, uint8_t slave) {
	if (i2c_start_mode(slave, TW_WRITE))
		return (0); /* ERROR */
	uint8_t	status;
	for (uint8_t i = 0; i < size; i++) {
		i2c_write(buffer[i]);
		status = TWSR & 0xF8;
		if (status != TW_MT_DATA_ACK) {
			uart_printstr(" == I2C: ERROR: expected TW_MT_DATA_ACK\r\n");
			i2c_stop();
			return (i);
		}
	}
	i2c_stop();
	return (size);
}

/**
 * @brief Send up to 255 bits of data to slave
 *
 * @param buffer -- Data to send in hexadecimale
 * @size -- buffer size
 * @slave -- Slave address
 * @return number of bytes sent
 */
uint8_t	i2c_send_hex(const char* buffer, uint8_t size, uint8_t slave) {
	uint8_t	start = 0;
	uint8_t	data;
	uint8_t	status;

	if (i2c_start_mode(slave, TW_WRITE))
		return (0);
	if (size > 2 && (buffer[1] == 'x' || buffer[1] == 'X'))
		start = 2;
	for (uint8_t i = start; i < size; i += 2) {
		data = 0;
		for (uint8_t j = 0; j < 2; j++) {
			if ('0' <= buffer[i + j] && buffer[i + j] <= '9')
				data |= buffer[i + j] - '0';
			if ('a' <= buffer[i + j] && buffer[i + j] <= 'z')
				data |= buffer[i + j] - 'a' + 10;
			if ('A' <= buffer[i + j] && buffer[i + j] <= 'Z')
				data |= buffer[i + j] - 'A' + 10;
			data = j == 0 ? data << 4 : data;
		}
		i2c_write(data);
		status = TWSR & 0xF8;
		if (status != TW_MT_DATA_ACK) {
			uart_printstr(" == I2C: ERROR: expected TW_MT_DATA_ACK\r\n");
			i2c_stop();
			return (i);
		}
	}
	i2c_stop();
	return (size);
}

/**
 * @brief Read up to 255 bytes of data
 *
 * @param buffer -- buffer to store data
 * @param size -- Expected masg len
 * @return number of byte read
 */
uint8_t	i2c_lread(unsigned char* buffer, uint8_t size) {
	// if (i2c_start_mode(slave, TW_READ))
	// 	return (0);
	uint8_t	status;
	for (uint8_t i = 0; i < size; i++) {
		buffer[i] = i2c_read_ack();
		status = TWSR & 0xF8;
		if (status != TW_MR_DATA_ACK) {
			uart_printstr(" == I2C: ERROR: expected TW_MR_DATA_ACK\r\n");
			i2c_stop();
			return (i);
		}
	}
	// i2c_stop();
	return (size);
}
