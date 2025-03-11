/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 22:58:57 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <avr/eeprom.h>
#include <stdint.h>
#include <util/delay.h>


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
/*                                   HEXDUMP                                  */
/* ************************************************************************** */

/**
 * @brief Convert hex string to number
 *
* @param buffer -- buffer to convert
* @param size -- buffer size
* @return uint32_t -- converted value
 */
uint32_t	str_to_hex(const unsigned char* buffer, uint8_t size) {
	uint32_t	val = 0;

	for (uint8_t i = 0; i < size; i++) {
		val <<= 4;
		if ('0' <= buffer[i] && buffer[i] <= '9')
			val |= buffer[i] - '0';
		else if ('a' <= buffer[i] && buffer[i] <= 'f')
			val |= buffer[i] - 'a' + 10;
		else if ('A' <= buffer[i] && buffer[i] <= 'F')
			val |= buffer[i] - 'A' + 10;
		else
			return 0xFFFFFFFF;
	}
	return (val);
}

/**
 * @brief Change one byte in eeprom
 */
void	change_eeprom_byte(void) {
	unsigned char	addr_buffer[5] = {0};
	unsigned char	data_buffer[5] = {0};
	uint32_t		addr = 0;
	uint32_t		data = 0;
	uint8_t			size;

	while (1) {
		for (uint8_t i = 0; i < 5; i++) {
			addr_buffer[i] = 0;
			data_buffer[i] = 0;
		}
		uart_printstr("Enter the address to change: ");
		size = uart_read_input((char *)addr_buffer, 5);
		addr = str_to_hex(addr_buffer, size);
		if (addr > 0x03FF) {
			uart_printstr("Invalid address\r\n\n");
			continue ;
		}
		uart_printstr("Enter the new value: ");
		size = uart_read_input((char *)data_buffer, 5);
		data = str_to_hex(data_buffer, size);
		if (data > 0xFF) {
			uart_printstr("Invalid value\r\n\n");
			continue ;
		}
		if (ft_eeprom_update_byte(addr, data))
			ft_hexdiff(addr);
		// else
		// 	ft_hexdump();
	}
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */
int main() {
	init();

	while (1) {
		/* READ TEMP sensor */
		change_eeprom_byte();
		_delay_ms(100);
	}

	return (0);
}
