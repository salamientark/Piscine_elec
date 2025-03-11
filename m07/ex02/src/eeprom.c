/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:44:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 19:24:02 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/io.h>
#include <stdint.h>

#define ADDR_WIDTH 7

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

/* ************************************************************************** */
/*                                   HEXDUMP                                  */
/* ************************************************************************** */

/**
 * @brief print address in hex format
 *
 * @param addr -- address
 * @param width -- address width (nbr of char print)
 */
void	print_addr(unsigned int addr, uint8_t width) {
	if (width != 0)
		print_addr(addr >> 4, width - 1);
	uart_tx((addr & 0xF) < 10 ? (addr & 0xF) + '0' : (addr & 0xF) + 'A' - 10);
}

/**
 * @brief print one memory line in hexdump format
 *
 * @param addr -- starting addr
 */
void	print_memory(unsigned int addr) {
	unsigned char	mem[16] = {0};

	print_addr(addr, ADDR_WIDTH);

	ft_eeprom_read_block(mem, addr, 16);
	for (uint8_t i = 0; i < 16; i++) {
		if (i % 2 == 0)
			uart_tx(' ');
		uart_printhex(mem[i]);
	}
}

/**
 * @brief print one memory line in hexdump -c format
 *
 * @param addr -- starting addr
 */
void	print_memory_c(unsigned int addr) {
	unsigned char	mem[16] = {0};

	ft_eeprom_read_block(mem, addr, 16);
	for (unsigned int i = 0; i < 16; i++) {
		if (mem[i] < ' ' || mem[i] > 0x7F)
			uart_tx('.');
		else
		 uart_tx(mem[i]);
	}
}

/**
 * @brief print one memory line in hexdump Showing modified bytes in red
 *
 * @param addr -- starting addr
 */
void	print_memory_diff(unsigned int start_addr, unsigned int diff_addr) {
	unsigned char	mem[16] = {0};

	print_addr(start_addr, ADDR_WIDTH);

	ft_eeprom_read_block(mem, start_addr, 16);
	for (uint8_t i = 0; i < 16; i++) {
		if (i % 2 == 0)
			uart_tx(' ');
		if (start_addr + i == diff_addr) {
			uart_printstr("\033[31m");
			uart_printhex(mem[i]);
			uart_printstr("\033[0m");
		}
		else
			uart_printhex(mem[i]);
	}
}

/**
 * @brief Hexdump of EEPROM
 */
void	ft_hexdump(void) {
	for (unsigned int i = 0; i < E2END; i+= 16) {
		print_memory(i);
		uart_printstr("\r\n");
		_delay_ms(50);
	}
}

/**
 * @brief print hexdump of EEPROM with diff
 *
 * @param addr -- address to show
 */
void	ft_hexdiff(uint32_t addr){
	for (unsigned int i = 0; i < E2END; i += 16) {
		if (addr >= i && addr < i + 16)
			print_memory_diff(i, addr);
		else
			print_memory(i);
		uart_printstr("\r\n");
		_delay_ms(10);
	}
} 

/**
 * @brief EEPROM Hexdump -C
 *
 * Hexdump in human readble format
 */
void	ft_hexdump_c(void) {
	for (unsigned int i = 0; i < E2END; i+= 16) {
		print_memory(i);
		uart_printstr(" |");
		print_memory_c(i);
		uart_printstr("|\r\n");
		_delay_ms(30);
	}
}
