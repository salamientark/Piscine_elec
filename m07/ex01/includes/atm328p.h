/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 22:53:30 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATM328P_H
# define ATM328P_H

# include <avr/io.h>
# include <avr/eeprom.h>
# include <util/delay.h>

# define ADDR_WIDTH 7

/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */
void	uart_init(void);
uint8_t	uart_rx(void);
void	uart_tx(const char c);
void	uart_printstr(const char* str);
void	uart_printhex(unsigned char c);
void	uart_printdec(unsigned int nbr);
uint8_t	uart_read_input(char* buffer, const uint8_t size);

/* ************************************************************************** */
/*                                  EEPROM                                     */
/* ************************************************************************** */
unsigned char	ft_eeprom_read_byte(unsigned int addr);
void			ft_eeprom_read_block(unsigned char *buffer, unsigned int addr, const unsigned int size);
void			ft_eeprom_write_byte(unsigned int addr, unsigned char data);
uint8_t			ft_eeprom_update_byte(unsigned int addr, unsigned char data);

/* ************************************************************************** */
/*                                 HEXDUMP                                    */
/* ************************************************************************** */
void			ft_hexdump(void);
void			ft_hexdiff(uint32_t addr);

#endif
