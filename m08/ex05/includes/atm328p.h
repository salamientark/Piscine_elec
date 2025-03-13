/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 11:48:33 by dbaladro         ###   ########.fr       */
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

# define SPI_OFF		0x00
# define SPI_RED		0x00FF0000
# define SPI_GREEN		0x0000FF00
# define SPI_BLUE		0x000000FF
# define SPI_YELLOW		0x00FFFF00
# define SPI_CYAN		0x0000FFFF
# define SPI_MAGENTA	0x00FF00FF

# define SPI_WHITE		0x00FFFFFF

# define READ_SIZE 33

/* ALIASES */
# define BUTTON1 PD2
# define BUTTON2 PD4

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
/*                                  BUTTON                                    */
/* ************************************************************************** */
void			button_init(void);
uint8_t			read_button(const uint8_t port, const uint8_t pin);

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
/*                                     ADC                                    */
/* ************************************************************************** */
void			adc_init(void);
uint8_t			adc_read(uint8_t ch);

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
