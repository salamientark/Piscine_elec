/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 01:21:02 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/eeprom.h>
#include <stdint.h>

#define ADDR_WIDTH 7

/*
 * EEPROM go from 0x00 to 0x3FF (1024 bytes)
 */

/* ************************************************************************** */
/*                                    INIT                                    */
/* ************************************************************************** */


/**
 * @brief Initialize mmcu
 */
void	init(void) {
	uart_init();
}

/* ************************************************************************** */
/*                                   EEPROM                                   */
/* ************************************************************************** */

// unsigned char	eeprom_read(unsigned int addr) {
// 	while (EECR & (1 << EEPE)) {} /* Wait until we can read/write EEPROM */
// 	EEAR = addr; /* Set the address register to the desired EEPROM address */
// 	EECR |= (1 << EERE); /* Start the read operation by setting EERE */
// 	return (EEDR); /* Return the data from the data register */
// }

/**
 * @brief print address in hex format
 *
 * @param addr -- address
 * @param width -- address width (nbr of char print)
 */
void	print_addr(uintptr_t addr, uint8_t width) {
	if (width != 0)
		print_addr(addr >> 4, width - 1);
	uart_tx((addr & 0xF) < 10 ? (addr & 0xF) + '0' : (addr & 0xF) + 'A' - 10);
}

/**
 * @brief print one memory line in hexdump format
 *
 * @param addr -- starting addr
 */
void	print_memory(uintptr_t addr) {
	uint8_t	mem[16] = {0};

	print_addr(addr, ADDR_WIDTH);

	eeprom_read_block((void *)mem, (const void *)(uintptr_t)addr, 16);
	for (uint8_t i = 0; i < 16; i++) {
		if (i % 2 == 0)
			uart_tx(' ');
		// uart_printhex(eeprom_read(addr + (uintptr_t)i));
		uart_printhex(mem[i]);
	}
	uart_printstr("\r\n");
}

/**
 * @brief Hexdump of EEPROM
 */
void	hexdump(void) {
	for (uintptr_t i = 0; i < E2END; i+= 16) {
		print_memory(i);
		for(volatile uint16_t i = 0; i < 0xFFFF; i++) {}
	}
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */


int main() {
	init();

	/* AHT20 Calibratiomn */
	while (1) {
		/* READ TEMP sensor */
		hexdump();
		uart_printstr("\r\n");
		for(volatile uint16_t i = 0; i < 0xFFFF; i++) {}
	}

	return (0);
}
