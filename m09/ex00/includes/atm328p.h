/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/14 13:41:26 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATM328P_H
# define ATM328P_H

# include <avr/io.h>
# include <util/delay.h>
# include "twi.h"

# define READ_SIZE		16

/* PCA9555 */
# define IO_EXPAND_ADDR 0x20

/* PCA9555 ALIASES */
#define PCA9555_I0 0x00 /* INPUT PORT 0 */
#define PCA9555_I1 0x01 /* INPUT PORT 1 */
#define PCA9555_O0 0x02 /* OUTPUT PORT 0 */
#define PCA9555_O1 0x03 /* OUTPUT PORT 1 */
#define PCA9555_P0 0x04 /* POLARITY INVERSION PORT 0 */
#define PCA9555_P1 0x05 /* POLARITY INVERSION PORT 0 */
#define PCA9555_C0 0x06 /* CONFIGURATION PORT 0 */
#define PCA9555_C1 0x07 /* CONFIGURATION PORT 1 */
#define PCA9555_INPUT 0
#define PCA9555_OUTPUT 1

/* EXPANDER PIN */
#define SW3		0x00
#define D11		0x01
#define D10		0x02
#define D9		0x03

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

/* ************************************************************************** */
/*                                  PCA9555                                   */
/* ************************************************************************** */
uint8_t			i2c_gpio_setio(const uint16_t val);
uint16_t		i2c_gpio_get_register_pair(const uint8_t reg);
void			i2c_gpio_set_register(const uint8_t reg, const uint8_t val);
void			i2c_gpio_set_register_pair(const uint8_t reg, const uint16_t data);
void			i2c_gpio_print_nbr(const uint8_t nbr);

#endif
