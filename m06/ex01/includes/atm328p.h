/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atm328p.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:38:17 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/10 20:18:08 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATM328P_H
# define ATM328P_H

# include <avr/io.h>
# include <util/twi.h>
# include <util/delay.h>

/* ************************************************************************** */
/*                                  TIMER                                     */
/* ************************************************************************** */
void	set_timer_ctc_ms(uint8_t timer, uint16_t delay);

/* ************************************************************************** */
/*                                   UART                                     */
/* ************************************************************************** */
void	uart_init(void);
uint8_t	uart_rx(void);
void	uart_tx(const char c);
void	uart_printstr(const char* str);
void	uart_printhex(unsigned char c);

/* ************************************************************************** */
/*                                   I2C                                      */
/* ************************************************************************** */
void	i2c_init(void);
void 	i2c_start(void);
void 	i2c_stop(void);
void	i2c_write(uint8_t data);
uint8_t	i2c_read_ack(void);
uint8_t	i2c_read_nack(void);

uint8_t i2c_start_mode(uint8_t slave, uint8_t mode);
uint8_t	i2c_send_str(const char* buffer, uint8_t size, uint8_t slave);
uint8_t	i2c_send_hex(const char* buffer, uint8_t size, uint8_t slave);
uint8_t	i2c_lread(unsigned char* buffer, uint8_t size);

#endif
