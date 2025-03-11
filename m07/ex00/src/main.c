/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 22:45:56 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/eeprom.h>
#include <stdint.h>

#define ADDR_WIDTH 7

/** *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 *    - Schem at Page 12
 *    - TIMER0 at Page 102-120
 *    - Interrupts at page 66 - 78
 *    - External Interrupts at Pages 79 - 84
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

/**
 * @brief Copy 16 bytes from src to dest
 *
 * @param src -- String to copy
 * @param dest -- Destination
 */
void	buffer_copy(unsigned char *dest, const unsigned char *src) {
	for(uint8_t i =0; i < 16; i++)
		dest[i] = src[i];
}

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
 * @brief Compare two 16bytes buffer
 *
 * @param buff_1 -- first buffer
 * @param buff_2 -- second buffer
 * @return 0 if equal, else buff_1 - buff_2
 */
uint8_t	buffer_cmp(const unsigned char *buff_1, const unsigned char *buff_2) {
	for (uint8_t i = 0; i < 16; i++)
		if (buff_1[i] != buff_2[i])
			return (buff_1[i] - buff_2[i]);
	return (0);
}

/**
 * @brief print one memory line in hexdump format
 *
 * @param addr -- starting addr
 */
void	print_memory(const unsigned char *data) {
	for (uint8_t i = 0; i < 16; i++) {
		if (i % 2 == 0)
			uart_tx(' ');
		uart_printhex(data[i]);
	}
}

/**
 * @brief Hexdump of EEPROM
 */
void	hexdump(void) {
	unsigned char	last_mem[16] = {0};
	unsigned char	mem[16] = {0};
	uint8_t			star = 0;
	
	for (uintptr_t i = 0; i < E2END; i+= 16) {
		ft_eeprom_read_block(mem, i, 16);
		if (!buffer_cmp(last_mem, mem)) { /* Same data */
			if (star)
				continue ;
			star = 1;
			uart_printstr("*\r\n");
			continue ;
		}

		print_addr(i, ADDR_WIDTH);
		print_memory(mem);
		uart_printstr("\r\n");
		buffer_copy(last_mem, mem);
		star = 0;
		for(volatile uint16_t i = 0; i < 0xFFFF; i++) {}
	}
	print_addr(E2END + 1, ADDR_WIDTH);
	uart_printstr("\r\n");
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */


int main() {
	init();

	while (1) {
		hexdump();
		uart_printstr("\r\n");
		for(volatile uint32_t i = 0; i < 0xFFFFF; i++) {}
	}

	return (0);
}
