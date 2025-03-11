/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 10:57:47 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATM328P_H
# define ATM328P_H

# include <avr/io.h>
# include <avr/eeprom.h>

/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */
void	uart_init(void);
uint8_t	uart_rx(void);
void	uart_tx(const char c);
void	uart_printstr(const char* str);
void	uart_printhex(unsigned char c);

/* ************************************************************************** */
/*                                  EEPROM                                     */
/* ************************************************************************** */
unsigned char	ft_eeprom_read_byte(unsigned int addr);
void			ft_eeprom_read_block(unsigned char *buffer, unsigned int addr, const unsigned int size);
void			ft_eeprom_write_byte(unsigned int addr, unsigned char data);

#endif
