/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 19:24:16 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATM328P_H
# define ATM328P_H

# include <avr/io.h>
# include <avr/eeprom.h>
# include <util/delay.h>

# define MAGIC_NBR 0x1F
# define DATA_MAX_SIZE 33
# define READ_BUFFER_SIZE 100

/* ************************************************************************** */
/*                                   C_LIB                                    */
/* ************************************************************************** */
uint8_t			is_limit(const unsigned int addr);
uint8_t			ft_strcmp(const unsigned char *s1, const unsigned char *s2);
uint8_t			ft_strncmp(const unsigned char *s1, const unsigned char *s2, unsigned int n);
unsigned int	ft_strlen(const unsigned char *str);
void			ft_strcat(unsigned char *s1, const unsigned char *s2);
void			ft_strcpy(unsigned char *dest, const unsigned char *src);
uint32_t		str_to_hex(const unsigned char* buffer, uint8_t size);

/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */
void			uart_init(void);
uint8_t			uart_rx(void);
void			uart_tx(const char c);
void			uart_printstr(const char* str);
void			uart_printhex(unsigned char c);
void			uart_printdec(unsigned int nbr);
uint8_t			uart_read_input(unsigned char* buffer, const uint8_t size);

/* ************************************************************************** */
/*                                  EEPROM                                    */
/* ************************************************************************** */
unsigned char	ft_eeprom_read_byte(unsigned int addr);
void			ft_eeprom_read_block(unsigned char *buffer, unsigned int addr,
					const unsigned int size);
void			ft_eeprom_write_byte(unsigned int addr, unsigned char data);
void			ft_eeprom_write_block(const unsigned char *buff, const unsigned int addr, const unsigned int size);
uint8_t			ft_eeprom_update_byte(unsigned int addr, unsigned char data);
void			ft_hexdump(void);
void			ft_hexdump_c(void);
void			ft_hexdiff(uint32_t addr);

/* ************************************************************************** */
/*                                    CMD                                     */
/* ************************************************************************** */

/* parser.c */
uint8_t			parse_cmd(const unsigned char *cmd, unsigned char *cmd_buff,
					unsigned char *prm_1_buff, unsigned char *prm_2_buff);

/* get_key_addr.c */
void			skip_data(unsigned int *i);
unsigned int	get_key_addr(const unsigned char *key);

/* cmd_read.c */
void			cmd_read(const unsigned char *key);
/* cmd_print.c */
void			cmd_print(void);
/* cmd_forget.c */
void			cmd_forget(const unsigned char *key);

#endif
