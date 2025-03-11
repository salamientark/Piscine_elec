/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexdump.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:47:28 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 23:08:25 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Copy 16 bytes from src to dest
 *
 * @param src -- String to copy
 * @param dest -- Destination
 */
void	buffer_copy(unsigned char *dest, const unsigned char *src) {
	for(uint8_t i =0; i < 16; i++)
		dest[i] = src[i];
}

/**
 * @brief Compare two 16bytes buffer
 *
 * @param buff_1 -- first buffer
 * @param buff_2 -- second buffer
 * @return 0 if equal, else buff_1 - buff_2
 */
uint8_t	buffer_cmp(const unsigned char *buff_1, const unsigned char *buff_2) {
	for (uint8_t i = 0; i < 16; i++)
		if (buff_1[i] != buff_2[i])
			return (buff_1[i] - buff_2[i]);
	return (0);
}

/**
 * @brief print address in hex format
 *
 * @param addr -- address
 * @param width -- address width (nbr of char print)
 */
void	print_addr(uintptr_t addr, uint8_t width) {
	if (width != 0)
		print_addr(addr >> 4, width - 1);
	uart_tx((addr & 0xF) < 10 ? (addr & 0xF) + '0' : (addr & 0xF) + 'A' - 10);
}

/**
 * @brief print one memory line in hexdump format
 *
 * @param addr -- starting addr
 */
void	print_memory(const unsigned char *data) {
	for (uint8_t i = 0; i < 16; i++) {
		if (i % 2 == 0)
			uart_tx(' ');
		uart_printhex(data[i]);
	}
}

/**
 * @brief print one memory line in hexdump -c format
 *
 * @param data -- data buffer
 */
void	print_memory_c(const unsigned char *data) {
	for (uint8_t i = 0; i < 16; i++) {
		if (data[i] < ' ' || data[i] > 0x7F)
			uart_tx('.');
		else
			uart_tx(data[i]);
	}
}

/**
 * @brief print one memory line in hexdump Showing modified bytes in red
 *
 * @param buff -- buffer to print
 * @param addr -- starting addr
 * @param diff_addr -- address to show in red
 */
void	print_memory_diff(const unsigned char *buff, uintptr_t start_addr, uintptr_t diff_addr) {
	for (uint8_t i = 0; i < 16; i++) {
		if (i % 2 == 0)
			uart_tx(' ');
		if (start_addr + i == diff_addr) {
			uart_printstr("\033[31m");
			uart_printhex(buff[i]);
			uart_printstr("\033[0m");
		}
		else
			uart_printhex(buff[i]);
	}
}

/**
 * @brief Hexdump of EEPROM
 */
void	ft_hexdump(void) {
	unsigned char	last_mem[16] = {0};
	unsigned char	mem[16] = {0};
	uint8_t			star = 0;
	
	for (uintptr_t i = 0; i < E2END; i+= 16) {
		ft_eeprom_read_block(mem, i, 16);
		if (!buffer_cmp(last_mem, mem)) { /* Same data */
			if (star)
				continue ;
			star = 1;
			uart_printstr("*\r\n");
			continue ;
		}

		print_addr(i, ADDR_WIDTH);
		print_memory(mem);
		uart_printstr("\r\n");
		buffer_copy(last_mem, mem);
		star = 0;
	}
	print_addr(E2END + 1, ADDR_WIDTH);
	uart_printstr("\r\n");
}

/**
 * @brief print hexdump of EEPROM with diff
 *
 * @param addr -- address to show
 */
void	ft_hexdiff(uint32_t addr){
	unsigned char	last_mem[16] = {0};
	unsigned char	mem[16] = {0};
	uint8_t			star = 0;

	for (uintptr_t i = 0; i < E2END; i += 16) {
		ft_eeprom_read_block(mem, i, 16);
		if (!buffer_cmp(last_mem, mem)) { /* Same data */
			if (star)
				continue ;
			star = 1;
			uart_printstr("*\r\n");
			continue ;
		}

		print_addr(i, ADDR_WIDTH);
		if (addr >= i && addr < i + 16)
			print_memory_diff(mem, i, addr);
		else
			print_memory(mem);
		uart_printstr("\r\n");
		buffer_copy(last_mem, mem);
		star = 0;
		_delay_ms(10);
	}
	print_addr(E2END + 1, ADDR_WIDTH);
	uart_printstr("\r\n");
} 

/**
 * @brief EEPROM Hexdump -C
 *
 * Hexdump in human readble format
 */
void	ft_hexdump_c(void) {
	unsigned char	last_mem[16] = {0};
	unsigned char	mem[16] = {0};
	uint8_t			star = 0;
	
	for (uintptr_t i = 0; i < E2END; i+= 16) {
		ft_eeprom_read_block(mem, i, 16);
		if (!buffer_cmp(last_mem, mem)) { /* Same data */
			if (star)
				continue ;
			star = 1;
			uart_printstr("*\r\n");
			continue ;
		}

		print_addr(i, ADDR_WIDTH);
		print_memory(mem);
		uart_printstr(" |");
		print_memory_c(mem);
		uart_printstr("|\r\n");
		_delay_ms(30);
		buffer_copy(last_mem, mem);
		star = 0;
	}
	print_addr(E2END + 1, ADDR_WIDTH);
	uart_printstr("\r\n");
}
