/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:44:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 23:19:12 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/io.h>
#include <stdint.h>

/**
 * @brief Read a byte from the EEPROM
 *
 * @param addr -- address to write to
 * @param data -- data to write
 */
unsigned char	ft_eeprom_read_byte(unsigned int addr) {
	while (EECR & (1 << EEPE)) {} /* Wait until we can read/write EEPROM */
	EEAR = addr; /* Set the address register to the desired EEPROM address */
	EECR |= (1 << EERE); /* Start the read operation by setting EERE */
	return (EEDR); /* Return the data from the data register */
}


/**
 * @brief Read a byte block from the EEPROM
 *
 * @param addr -- address to write to
 * @param data -- data to write
 */
void	ft_eeprom_read_block(unsigned char *buffer, unsigned int addr, const unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		buffer[i] = ft_eeprom_read_byte(addr + i);
	}
}

/**
 * @brief Write a byte to the EEPROM
 *
 * @param addr -- address to write to
 * @param data -- data to write
 */
void	ft_eeprom_write_byte(unsigned int addr, unsigned char data) {
	while (EECR & (1 << EEPE)) {} /* Wait until we can read/write EEPROM */
	EEAR = addr; /* Set the address register to the desired EEPROM address */
	EEDR = data; /* Set the data register to the desired value */
	EECR |= (1 << EEMPE); /* Enable master write */
	EECR |= (1 << EEPE); /* Start the write operation by setting EEPE */
}

/**
 * @brief Write a byte to the EEPROM if the value is different
 *
 * @param addr -- address to write to
 * @param data -- data to write
* @return uint8_t -- 1 if the value was updated, 0 otherwise
 */
uint8_t	ft_eeprom_update_byte(unsigned int addr, unsigned char data) {
	if (ft_eeprom_read_byte(addr) != data) {
		ft_eeprom_write_byte(addr, data);
		return (1);
	}
	return (0);
}

/**
 * @brief Write a full data block in the EEPROM
 *
 * @param buff -- buffer containing data
 * @param addr -- EEPROM start address
 * @param size -- data size
 */
void ft_eeprom_write_block(const unsigned char *buff, const unsigned int addr, const unsigned int size) {
	for (unsigned int i = 0; i < size; i++)
		ft_eeprom_write_byte(addr + i, buff[i]);
}
