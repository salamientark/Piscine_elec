/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:44:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 10:57:23 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/io.h>

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
