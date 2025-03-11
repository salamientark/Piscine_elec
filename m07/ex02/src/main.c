/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 17:15:27 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <stdint.h>


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
		size = uart_read_input((unsigned char *)addr_buffer, 5);
		addr = str_to_hex(addr_buffer, size);
		if (addr > 0x03FF) {
			uart_printstr("Invalid address\r\n\n");
			continue ;
		}
		uart_printstr("Enter the new value: ");
		size = uart_read_input((unsigned char *)data_buffer, 5);
		data = str_to_hex(data_buffer, size);
		if (data > 0xFF) {
			uart_printstr("Invalid value\r\n\n");
			continue ;
		}
		if (ft_eeprom_update_byte(addr, data))
			ft_hexdiff(addr);
		else
			ft_hexdump();
	}
}

void	test_print_cmd(const unsigned char *cmd, const unsigned char *prm_1, const unsigned char *prm_2) {
	uart_printstr("[{ ");
	uart_printstr((const char *)cmd);
	uart_printstr(" } | { ");
	uart_printstr((const char *)prm_1);
	uart_printstr(" } | { ");
	uart_printstr((const char *)prm_2);
	uart_printstr(" }]\r\n\n");
}


void	exec_cmd(const unsigned char *cmd) {
	unsigned char	cmd_buffer[32] = {0};
	unsigned char	prm_1_buffer[32] = {0};
	unsigned char	prm_2_buffer[32] = {0};

	if (parse_cmd(cmd, cmd_buffer, prm_1_buffer, prm_2_buffer)) {
		uart_printstr("\033[31mInvalid cmd\033[0m\r\n");
	}
	else
		uart_printstr("\033[32mValid cmd\033[0m\r\n");
	test_print_cmd(cmd_buffer, prm_1_buffer, prm_2_buffer);
}
	

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */
int main() {
	init();

	unsigned char	buffer[DATA_MAX_SIZE] = {0};

	uart_printstr("==================== WELCOME ===================\r\n\n");

	while (1) {
		for (uint8_t i = 0; i < DATA_MAX_SIZE; i++)
			buffer[i] = 0;
		uart_printstr("> ");
		uart_read_input((unsigned char *)buffer, DATA_MAX_SIZE);
		exec_cmd(buffer);
	}
	
	return (0);
}
