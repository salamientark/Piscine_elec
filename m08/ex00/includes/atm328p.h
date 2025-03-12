/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/12 19:30:34 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATM328P_H
# define ATM328P_H

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

# define ADA102_DATA_SIZE 4 /* Data fame size according to datasheet */
# define SS PB2 /* Define Slave Select */
# define MISO PB4 /* SPI MISO PIN */
# define MOSI PB3 /* SPI MOSI PIN */
# define SCK PB5 /* SPI CLOCK PIN */

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
/*                                    SPI                                     */
/* ************************************************************************** */
/* spi.c */
void			spi_init_master(void);
void			spi_transmit_byte(const unsigned char byte);
void			spi_transmit_block(const unsigned char *block, const unsigned int size);

/* led_ada102.c */
void			spi_led_send_start(void);
void			spi_led_send_stop(void);
void			spi_led_set_data_frame(unsigned char *frame, const uint8_t brightness,
					const uint8_t r, const uint8_t g, const uint8_t b);
void			spi_set_led(const uint32_t led_1, const uint32_t led_2, const uint32_t led_3);
#endif
