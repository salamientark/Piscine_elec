/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:11:55 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 18:26:13 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Pritn the value associated to key
 *
 * WARNING addr must point to the beginning of the key.
 * If not, UNDEFINED BEHAVIOUR
 */
void	print_value(const unsigned int addr) {
	unsigned int	index = addr;

	if (is_limit(index))
		index += 2;
	while (ft_eeprom_read_byte(index) != MAGIC_NBR)
		index++;
	index++;
	while (ft_eeprom_read_byte(index) != MAGIC_NBR)
		uart_tx(ft_eeprom_read_byte(index++));
}

/**
 * @brief Print value associted to key found on EEPROM
 *
 * @param key -- key
 */
void	cmd_read(const unsigned char *key) {
	unsigned int	addr = get_key_addr(key);

	if (addr == 0xFFFF)
		return (uart_printstr("empty\r\n"));
	return (print_value(addr));
}
