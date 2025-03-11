/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:39:32 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 19:54:43 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"
#include <stdint.h>

void	create_data_block(unsigned char *block, const unsigned char *key,
		const unsigned char *val) {
	unsigned int	block_i = 0;

	block[block_i++] = MAGIC_NBR;
	block[block_i++] = MAGIC_NBR;
	for (unsigned int i = 0; key[i]; i++)
		block[block_i++] = key[i];
	block[block_i++] = MAGIC_NBR;
	for (unsigned int i = 0; val[i]; i++)
		block[block_i++] = val[i];
	block[block_i++] = MAGIC_NBR;
	block[block_i] = MAGIC_NBR;
}

/**
 * @brief Check if enough space starting at addr
 *
 * @param addr -- start address pointer
 * @param data_len -- data size to insert (including MAGIC NUMBERS)
 */
uint8_t	enough_space(unsigned int *addr, const unsigned int data_len) {
	for (unsigned int i = 0; i < data_len; i++) {
		if (is_limit(*addr + i)) {
			*addr += i;
			return (0);
		}
	}
	return (1);
}

/**
 * @brief Find free space for data in the EEPROM
 *
 * @param data_len -- len of the data (including MAGIC NUMBERS)
 */
unsigned int	find_free_addr(const unsigned int data_len) {
	unsigned int	i = 0;

	while (i < E2END - data_len) {
		if (is_limit(i)) {
			skip_data(&i);
			i++;
			continue ;
		}
		if (enough_space(&i, data_len))
			return (i);
		
	}
	return (0xFFFF);
}

/**
 * @brief write key|data to EEPROM
 *
 * If no space found "no space"
 *
 * @param key -- The key to insert
 * @param val -- The value to insert
 */
void	cmd_write(const unsigned char *key, const unsigned char *val) {
	unsigned char	buff[70] = {0}; /* 32 max data byte * 2 + sep + 2 * lim */
	unsigned char	len = 0;
	unsigned int	addr = 0;

	/* Check if key already exist */
	if (get_key_addr(key) != 0xFFFF)
		return (uart_printstr("already exist\r\n"));
	/* Create data block */
	create_data_block(buff, key, val);
	// uart_print_strtohex(buff);
	while (buff[len])
		len++;
	uart_printstr("len: ");
	uart_printdec(len);
	uart_printstr("\r\n");
	
	addr = find_free_addr(len);
	if (addr == 0xFFFF)
		return (uart_printstr("no space left\r\n"));
	/* Write block */
	ft_eeprom_write_block(buff, addr, len);
	uart_printstr("0x");
	uart_printhex(addr >> 8);
	uart_printhex(addr & 0xFF);
	uart_printstr("\r\n");
}
