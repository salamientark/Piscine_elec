/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:11:55 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 16:37:31 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Get the key at given address
 *
 * WARNING addr must be a begining of a key.
 * If not, undefined behavior
 *
 * @param addr -- key addr in EEPROM
 * @param buffer -- buffer to save the key
 */
void get_key(const unsigned int addr, char *buffer) {
	unsigned int	ref = addr;
	unsigned int	index = 0;


	if (is_limit(ref))
		ref += 2;
	while (ref + index < E2END && ft_eeprom_read_byte(ref + index) != MAGIC_NBR) {
		buffer[index] = ft_eeprom_read_byte(ref + index);
		index++;
	}
}

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
 * @brief skip a key/value pair by updating the address
 *
 * @param i -- adress ptr
 */
void	skip_data(unsigned int *i) {
	if (is_limit(*i))
		i += 2;
	while (!is_limit(*i))
		i++;
	i++;
}

/**
 * @brief Print value associted to key found on EEPROM
 *
 * @param key -- key
 */
void	cmd_read(const unsigned char *key) {
	unsigned int	i = 0;
	unsigned int	key_len = ft_strlen(key);
	unsigned int	eeprom_data[DATA_MAX_SIZE] = {0};
	
	while (i < E2END - key_len) {
		if (is_limit(i)) {
			get_key(i, (char *)eeprom_data);
			if (!ft_strncmp(key, (unsigned char *)eeprom_data, key_len))
				return (print_value(i));
			skip_data(&i);
		}
		i++;
	}
	uart_printstr("empty\r\n");
}
