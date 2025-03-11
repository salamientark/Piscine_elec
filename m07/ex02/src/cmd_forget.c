/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_forget.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:14:48 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 18:32:24 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Erase key|value data from EEPROM
 *
 * @param key -- The key to search for
 */
void	cmd_forget(const unsigned char *key) {
	unsigned int	addr = get_key_addr(key);

	if (addr == 0xFFFF)
		return (uart_printstr("not found\r\n"));
	ft_eeprom_write_byte(addr++, 0xFF);
	ft_eeprom_write_byte(addr++, 0xFF);
	while (!is_limit(addr))
		ft_eeprom_write_byte(addr++, 0xFF);
	ft_eeprom_write_byte(addr++, 0xFF);
	ft_eeprom_write_byte(addr, 0xFF);
}
