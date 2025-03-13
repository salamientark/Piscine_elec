/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 11:18:18 by dbaladro         ###   ########.fr       */
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

// void	test_print_cmd(const unsigned char *cmd, const unsigned char *prm_1, const unsigned char *prm_2) {
// 	uart_printstr("[{ ");
// 	uart_printstr((const char *)cmd);
// 	uart_printstr(" } | { ");
// 	uart_printstr((const char *)prm_1);
// 	uart_printstr(" } | { ");
// 	uart_printstr((const char *)prm_2);
// 	uart_printstr(" }]\r\n\n");
// }


void	exec_cmd(const unsigned char *cmd) {
	unsigned char	cmd_buffer[32] = {0};
	unsigned char	prm_1_buffer[DATA_MAX_SIZE] = {0};
	unsigned char	prm_2_buffer[DATA_MAX_SIZE] = {0};

	if (parse_cmd(cmd, cmd_buffer, prm_1_buffer, prm_2_buffer) != 0)
		return (uart_printstr("\033[31mInvalid cmd\033[0m\r\n"));
	// uart_printstr("\033[32mValid cmd\033[0m\r\n");
	// test_print_cmd(cmd_buffer, prm_1_buffer, prm_2_buffer);
	if (!ft_strcmp((const unsigned char *)"PRINT", cmd_buffer))
		return (cmd_print());
	if (!ft_strcmp((const unsigned char *)"READ", cmd_buffer))
		return (cmd_read(prm_1_buffer));
	if (!ft_strcmp((const unsigned char *)"FORGET", cmd_buffer))
		return (cmd_forget(prm_1_buffer));
	if (!ft_strcmp((const unsigned char *)"WRITE", cmd_buffer))
		return (cmd_write(prm_1_buffer, prm_2_buffer));
}
	

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */
int main() {
	init();

	unsigned char	buffer[READ_BUFFER_SIZE] = {0};

	// uart_printstr("==================== WELCOME ===================\r\n\n");

	while (1) {
		for (uint8_t i = 0; i < READ_BUFFER_SIZE; i++)
			buffer[i] = 0;
		uart_printstr("> ");
		uart_read_input((unsigned char *)buffer, READ_BUFFER_SIZE);
		exec_cmd(buffer);
	}
	
	return (0);
}
