/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 10:33:58 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

#define AHT20 0x38
#define ERROR 0xFF

uint8_t g_i2c_status;

/* ************************************************************************** */
/*                                    INIT                                    */
/* ************************************************************************** */


/**
 * @brief Initialize mmcu
 */
void	init(void) {
	uart_init();
	i2c_init();
}


/* ************************************************************************** */
/*                                     I2C                                    */
/* ************************************************************************** */

/**
 * @brief Print hex value to UART
 *
 * @param c -- char
 */
void	print_hex_value(unsigned char c) {
	uart_tx((c >> 4) < 10 ? (c >> 4) + '0' : (c >> 4) + 'A' - 10);
	uart_tx((c & 0xF) < 10 ? (c & 0xF) + '0' : (c & 0xF) + 'A' - 10);
}

/**
 * @brief Print hex value to UART
 *
 * @param str -- char
 */
void	uart_print_strtohex(const unsigned char *buffer, uint8_t size) {
	for (uint8_t i = 0; i < size; i++) {
		print_hex_value(buffer[i]);
		uart_tx(' ');
	}
}
/* ************************************************************************** */
/*                                 CALIBRATION                                */
/* ************************************************************************** */

/**
 * @brief Check the AHT20 calibration bit
 *
 * @return 1 if calibration is already done, else 0
 */
uint8_t	check_calibration(void) {
	uint8_t	data;

	uart_printstr(" == Check AHT20 Calibration\r\n");
	i2c_start_mode(AHT20, TW_WRITE);
	uart_printstr(" == I2C: MODE SLA_W: 0x38\r\n");
	i2c_write(0x71);
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_MT_DATA_ACK) {
		uart_printstr(" == I2C: ERROR: expected TW_MT_DATA_ACK\r\n");
		return (ERROR);
	}
	uart_printstr(" == I2C: SENDED: 0x71 (Get calibration register)\r\n");
	i2c_start_mode(AHT20, TW_READ);
	data = i2c_read_nack();
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_MR_DATA_NACK) { /* Check aknoledgment */
		uart_printstr(" == I2C: ERROR: expected TW_MR_DATA_ACK\r\n");
		return (ERROR);
	}
	uart_printstr(" == I2C: Calibration register value : 0x");
	print_hex_value(data);
	uart_printstr("\r\n");
	i2c_stop();
	return (data & (1 << 3));
}

void	caliber(void) {
	/* Start a write conversation with AHT20 */
	i2c_start();
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_START && g_i2c_status != TW_REP_START) {
		uart_printstr(" == I2C: ERROR: expected TW_START or TW_REP_START\r\n");
		g_i2c_status = ERROR;
		return ;
	}
	uart_printstr(" == I2C: START\r\n");
	/* Address packet */
	i2c_write(AHT20 << 1 | TW_WRITE);
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_MT_SLA_ACK) { /* Check aknoledgment */
		uart_printstr("Couldn't reach target on address: 0x");
		print_hex_value(AHT20);
		uart_printstr("\r\n");
		g_i2c_status = ERROR;
		return ;
	}
	uart_printstr(" == I2C: MODE SLA_W: 0x38\r\n");
	i2c_write(0xBE);
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_MT_DATA_ACK) {
		uart_printstr(" == I2C: ERROR: expected TW_MT_DATA_ACK\r\n");
		g_i2c_status = ERROR;
		return ;
	}
	i2c_write(0x08);
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_MT_DATA_ACK) {
		uart_printstr(" == I2C: ERROR: expected TW_MT_DATA_ACK\r\n");
		g_i2c_status = ERROR;
		return ;
	}
	i2c_write(0x00);
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_MT_DATA_ACK) {
		uart_printstr(" == I2C: ERROR: expected TW_MT_DATA_ACK\r\n");
		g_i2c_status = ERROR;
		return ;
	}
	uart_printstr(" == I2C: SENDED: 0xBE (Caliber temp sensor)\r\n");
	_delay_ms(10);
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

void i2c_read(void) {
	unsigned char	buffer[6] = {0};

	i2c_send_hex("AC3300", 6, AHT20);

	_delay_ms(80); /* Sensor measurment time */

	i2c_start_mode(AHT20, TW_READ);
	/* Wait for measurment complete */
	buffer[0] = i2c_read_ack();
	g_i2c_status = TWSR & 0xF8;
	if (g_i2c_status != TW_MR_DATA_ACK) {
		uart_printstr(" == I2C: ERROR: expected TW_MR_DATA_ACK\r\n");
		return ;
	}
	while (buffer[0] & 0x80) {
		buffer[0] = i2c_read_ack();
		g_i2c_status = TWSR & 0xF8;
		if (g_i2c_status != TW_MR_DATA_ACK) {
			uart_printstr(" == I2C: ERROR: expected TW_MR_DATA_ACK\r\n");
			return ;
		}
	}
	print_hex_value(buffer[0]);
	uart_tx(' ');

	i2c_lread(buffer, 6);
	uart_print_strtohex((const unsigned char *)buffer, 6);
	uart_printstr("\r\n");
}

int main() {
	init();

	/* AHT20 Calibratiomn */
	_delay_ms(40);
	if (!check_calibration()) {
		g_i2c_status = ERROR;
		while (g_i2c_status == ERROR)
			caliber();
	}

	while (1) {
		/* READ TEMP sensor */
		i2c_read();
		i2c_stop();

		/* STOP request */
		_delay_ms(1000);
	}

	return (0);
}
