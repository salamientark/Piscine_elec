/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:12:54 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 00:18:23 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"


/**
 * @brief Skip space in cmd starting from pos
 *
 * @param cmd -- Readline result
 * @param pos -- position in cmd
 */
void	skip_space(const unsigned char *cmd, uint8_t *pos) {
	while (*pos < READ_SIZE && cmd[*pos] == ' ')
		(*pos)++;
}

/**
 * @brief Check if buff contain one valid hex value
 *
 * @param buff -- buffer to check
 * @param size -- buff size
 * @return uint8_t -- 1 if valid hex value, else 0
 */
uint8_t	is_valid_hex(const unsigned char *buff, const uint8_t size) {
	for (uint8_t i = 0; i < size; i++) {
		if (!(('0' <= buff[i] && buff[i] <= '9')
			|| ('a' <= buff[i] && buff[i] <= 'f')
			|| ('A' <= buff[i] && buff[i] <= 'F')))
				return (0);
	}
	return (1);
}

/**
 * @brief Check if data is valid by applying a mask
 *
 * Check size and search for 'D' at the right place
 * @param buff -- Read buffer
 * @param pos -- buffer pos
 * @return uint8_t -- 1 if mask is valid, else 0
 */
uint8_t	valid_mask(const unsigned char *buff, const unsigned int pos) {
	if (pos > READ_SIZE - 8) /* Prtection for invalid access */
		return (0);
	uart_printstr("protection overflow OK\r\n");
	uart_printhex(buff[pos + 8]);
	if (buff[pos + 8] != 0 && buff[pos + 8] != ' ' && buff[pos + 8] != '\r')
		return (0);
	uart_printstr("Size OK\r\n");
	if (buff[pos + 6] != 'D')
		return (0);
	uart_printstr("D indicator OK\r\n");
	if (!('6' <= buff[pos + 7] && buff[pos + 7] <= '8'))
		return (0);
	uart_printstr("Led number valid OK\r\n");
	return (1);
}

/**
 * @brief Parse readed input
 *
 * Return a semi-parsed value the entire buffer is converted to hex number,
 * including the DX at the end
 * The end must be re-parsed
 *
 * @param buff -- read buffer
 * @return 0 on error, return 0xFF on FULLRAINBOW mode, else parsed value
 */
uint32_t	parse(const unsigned char *buff) {
	uint8_t		i = 0;
	uint32_t	res;
	
	skip_space(buff, &i);
	if (buff[i++] != '#')
		return (uart_printstr("\033[31mInvalid input\033[0m\r\n"), 0);

	uart_printstr("Valid #\r\n");

	if (!ft_strncmp((const unsigned char *)"FULLRAINBOW", buff + i, 10)) {
		i += 11;
		skip_space(buff, &i);
		uart_tx(buff[i]);
		if (buff[i] != 0 && buff[i] != '\r')
			return (uart_printstr("\033[31mInvalid input\033[0m\r\n"), 0);
		TIMSK0 |= (1 << OCIE0A); /* Set interrupts to activat Rainbow */
		return (0xFF);
	}
	uart_printstr("Not FULLRAINBOW #\r\n");
	if (!valid_mask(buff, i))
		return (uart_printstr("\033[31mInvalid input\033[0m\r\n"), 0);
	uart_printstr("Valid mask\r\n");
	if (!is_valid_hex(buff + i, 8))
		return (uart_printstr("\033[31mInvalid input\033[0m\r\n"), 0);
	uart_printstr("Valid HEX\r\n");
	res = str_to_hex(buff + i, 8);
	i += 8;
	skip_space(buff, &i);
	uart_printhex(buff[i]);
	if (buff[i] != 0 && buff[i] != '\r')
		return (uart_printstr("\033[31mInvalid input\033[0m\r\n"), 0);
	uart_printstr("\033[32mValid INPUT\033[0m\r\n");
	TIMSK0 &= ~(1 << OCIE0A);
	return (res);
}
