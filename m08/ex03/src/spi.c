/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:29:03 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/12 19:18:41 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * LED datasheet can be found at :
 * - https://www.mouser.com/datasheet/2/737/APA102_2020_SMD_LED-2487271.pdf?srsltid=AfmBOoolR8OmnxNRM0r5-GDEkImscvaFJbNFU7taU9-UEQY_EwylDZLt
 *
 */

#include "../includes/atm328p.h"

/* ************************************************************************** */
/*                                   SPI                                      */
/* ************************************************************************** */

/**
 * @brief SPI Set mmcu as Master
 */
void	spi_init_master(void) {
	/* Configure PORT for SPI communiccation */
	DDRB	|= (1 << MOSI) /* Output for SPI
						   * MOSI: Master Out Slave In */
			| (1 << SCK) /* Output for SCK
						  * Master clock for SPI */
			| (1 << SS); /* Set Slave Selcet pin as output */
	/* Configure SPI Control register */
	SPCR	= (1 << SPE) /* Enable SPI */
			| (1 << MSTR) /* Set Master SPI */
			| (1 << SPR0); /* Set clock to 1Mhz (1000Khz) Page 3 datasheet */
	
}

/**
 * @brief Send SPI data when configured as master
 *
 * @param byte -- data byte to send
 */
void	spi_transmit_byte(const unsigned char byte) {

	SPDR = byte; /* Send data by putting in in register */
	while (!(SPSR & (1 << SPIF))) {} /* Wait for transmit to be complete */
}

/**
 * @brief Send SPI data MSB first when configured as master
 *
 * @param byte -- data byte to send
 */
void	spi_transmit_byte_msb(const unsigned char byte) {
	SPCR &= ~(1 << DORD); /* Set MSB first send */
	SPDR = byte; /* Send data by putting in in register */
	while (!(SPSR & (1 << SPIF))) {} /* Wait for transmit to be complete */
}

/**
 * @brief Send SPI data LSB first when configured as master
 *
 * @param byte -- data byte to send
 */
void	spi_transmit_byte_lsb(const unsigned char byte) {
	SPCR |= (1 << DORD); /* Set MSB first send */
	SPDR = byte; /* Send data by putting in in register */
	while (!(SPSR & (1 << SPIF))) {} /* Wait for transmit to be complete */
}

/**
 * @brief SPI transmit a full block of data
 *
 * @param block -- data block
 * @param size -- block size
 */
void	spi_transmit_block(const unsigned char *block, const unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		spi_transmit_byte(block[i]);
	}
}
