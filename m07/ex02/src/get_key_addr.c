/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_key_addr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:26:02 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 19:12:48 by dbaladro         ###   ########.fr       */
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
 * @brief Get key address from EEPROM
 * @param key -- Key to search in EEPROM
 * @return unsigned int -- key address if found, else 0xFFFFFFFF
 */
unsigned int	get_key_addr(const unsigned char *key) {
	unsigned int	key_len = ft_strlen(key);
	unsigned int	addr = 0;
	unsigned int	eeprom_data[DATA_MAX_SIZE] = {0};

	while (addr < E2END - key_len) {
		if (is_limit(addr)) {
			get_key(addr, (char *)eeprom_data);
			if (!ft_strncmp(key, (unsigned char *)eeprom_data, key_len))
				return (addr);
			skip_data(&addr);
		}
		addr++;
	}
	return (0xFFFF); /* Key not found */
}
