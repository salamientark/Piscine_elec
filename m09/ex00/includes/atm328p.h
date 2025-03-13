/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 18:18:49 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATM328P_H
# define ATM328P_H

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>
# include <util/twi.h>

# define READ_SIZE		16

/* ************************************************************************** */
/*                                   C_LIB                                    */
/* ************************************************************************** */
uint8_t			is_limit(const unsigned int addr);
uint8_t			ft_strcmp(const unsigned char *s1, const unsigned char *s2);
uint8_t			ft_strncmp(const unsigned char *s1, const unsigned char *s2, unsigned int n);
unsigned int	ft_strlen(const unsigned char *str);
void			ft_strcat(unsigned char *s1, const unsigned char *s2);
void			ft_strcpy(unsigned char *dest, const unsigned char *src);
void			ft_memset(unsigned char *buff, const unsigned int byte,
					const unsigned int size);
uint32_t		str_to_hex(const unsigned char* buffer, uint8_t size);

/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */
void			uart_init(void);
uint8_t			uart_rx(void);
void			uart_tx(const char c);
void			uart_printstr(const char* str);
void			uart_printhex(unsigned char c);
void			uart_print_strtohex(const unsigned char *str);
void			uart_printdec(unsigned int nbr);
uint8_t			uart_read_input(unsigned char* buffer, const uint8_t size);

/* ************************************************************************** */
/*                                   PARSER                                   */
/* ************************************************************************** */
uint32_t		parse(const unsigned char *buff);

/* ************************************************************************** */
/*                                    i2C                                     */
/* ************************************************************************** */
void			i2c_init(void);
void 			i2c_start(void);
void 			i2c_stop(void);
void			i2c_write(uint8_t data);
uint8_t			i2c_read_ack(void);
uint8_t			i2c_read_nack(void);
uint8_t 		i2c_start_mode(uint8_t slave, uint8_t mode);
uint8_t			i2c_send_block(const char* block, uint8_t size, uint8_t slave);
uint8_t			i2c_send_hex(const char* buffer, uint8_t size, uint8_t slave);
uint8_t			i2c_lread(unsigned char* buffer, uint8_t size);
uint8_t			i2c_ping_addr(uint8_t addr);
void			i2c_ping(void);

#endif
