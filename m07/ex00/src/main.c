/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 10:58:32 by dbaladro         ###   ########.fr       */
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
	unsigned char	mem[16] = {0};

	print_addr(addr, ADDR_WIDTH);

	ft_eeprom_read_block(mem, addr, 16);
	for (uint8_t i = 0; i < 16; i++) {
		if (i % 2 == 0)
			uart_tx(' ');
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

	while (1) {
		/* READ TEMP sensor */
		hexdump();
		uart_printstr("\r\n");
		for(volatile uint16_t i = 0; i < 0xFFFF; i++) {}
	}

	return (0);
}
