/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:18:41 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 17:45:13 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <stdint.h>

/**
 * @brief Check if the address is a start or end of key/value
 *
 * @param addr -- address to check
 * @return uint8_t -- 1 if the address is a limit, 0 otherwise
 */
uint8_t	is_limit(const unsigned int addr) {
	if (addr >= E2END)
		return (0);
	if (ft_eeprom_read_byte(addr) != MAGIC_NBR)
		return (0);
	return (ft_eeprom_read_byte(addr + 1) == MAGIC_NBR);
}

/**
 * @brief Reworked ft_strlen for atmega328p EEPROM operation
 *
 * @param str -- string
 * @return unsigned int -- len
 */
unsigned int	ft_strlen(const unsigned char *str) {
	int	len = 0;

	if (!str)
		return (len);
	while (str[len] && str[len] != MAGIC_NBR)
		len++;
	return (len);
}

/**
 * @brief Reworked ft_strcmp for atmega328p EEPROM operation
 *
 * @param s1 -- first word to compare
 * @param s2 -- second word to compare
 * @return 0 if equal, else s1 - s2
 */
uint8_t	ft_strcmp(const unsigned char *s1, const unsigned char *s2) {
	uint8_t	i = 0;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (s2[0]);
	if (!s2)
		return (s1[0]);
	while (s1[i] && s2[i]) {
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;

	}
	return (s1[i] - s2[i]);
}


/**
 * @brief Reworked ft_strncmp for atmega328p EEPROM operation
 *
 * @param s1 -- first word to compare
 * @param s2 -- second word to compare
 * @param n -- word size
 * @return 0 if equal, else s1 - s2
 */
uint8_t	ft_strncmp(const unsigned char *s1, const unsigned char *s2, unsigned int n) {
	unsigned int	i = 0;

	if (!s1)
		return (s2[i]);
	if (!s2)
		return (s1[i]);
	while (i < n) {
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

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
		if ('a' <= buffer[i] && buffer[i] <= 'f')
			val |= buffer[i] - 'a' + 10;
		if ('A' <= buffer[i] && buffer[i] <= 'F')
			val |= buffer[i] - 'A' + 10;
	}
	return (val);
}
