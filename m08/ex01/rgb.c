/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:48:04 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/12 19:50:16 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

# define ADA102_DATA_SIZE 4 /* Data fame size according to datasheet */
# define SS PB2 /* Define Slave Select */
# define MISO PB4 /* SPI MISO PIN */
# define MOSI PB3 /* SPI MOSI PIN */
# define SCK PB5 /* SPI CLOCK PIN */

# define SPI_OFF		0x00
# define SPI_RED		0x1FFF0000
# define SPI_BLUE		0x1F00FF00
# define SPI_GREEN		0x1F0000FF
# define SPI_YELLOW		0x1FFFFF00
# define SPI_CYAN		0x1F00FFFF
# define SPI_MAGENTA	0x1FFF00FF

# define SPI_WHITE 0x1FFFFFFF

/**
 * @brief Fill the buffer with the given byte
 *
 * @param buff -- buffer to fill
 * @param byte -- Data to fill the buffer with
 * @param size -- size of the buffer
 */
void	ft_memset(unsigned char *buff, const unsigned int byte,
		const unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		buff[i] = byte;
	}
}

/**
 * @brief SPI Set mmcu as Master
 */
void	spi_init_master(void) {
	/* Configure PORT for SPI communiccation */
	DDRB	|= (1 << MOSI) /* Output for SPI
						   * MOSI: Master Out Slave In */
			| (1 << SCK) /* Output for SCK
						  * Master clock for SPI */
			| (1 << SS); /* Set Slave Selcet pin as output */
	/* Configure SPI Control register */
	SPCR	= (1 << SPE) /* Enable SPI */
			| (1 << MSTR) /* Set Master SPI */
			| (1 << SPR0); /* Set clock to 1Mhz (1000Khz) Page 3 datasheet */
	
}

/**
 * @brief Send SPI data when configured as master
 *
 * @param byte -- data byte to send
 */
void	spi_transmit_byte(const unsigned char byte) {

	SPDR = byte; /* Send data by putting in in register */
	while (!(SPSR & (1 << SPIF))) {} /* Wait for transmit to be complete */
}


/**
 * @brief SPI transmit a full block of data
 *
 * @param block -- data block
 * @param size -- block size
 */
void	spi_transmit_block(const unsigned char *block, const unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		spi_transmit_byte(block[i]);
	}
}

/**
 * @brief Send START data frame for ADA102 led on SPI
 */
void	spi_led_send_start(void) {
	unsigned char	block[ADA102_DATA_SIZE];
	ft_memset(block, 0x00, ADA102_DATA_SIZE);

	spi_transmit_block(block, ADA102_DATA_SIZE);
}

/**
 * @brief Send START data frame for ADA102 led on SPI
 */
void	spi_led_send_stop(void) {
	unsigned char	block[ADA102_DATA_SIZE];
	ft_memset(block, 0xFF, ADA102_DATA_SIZE);

	spi_transmit_block(block, ADA102_DATA_SIZE);
}

/**
 * @brief Set led brightness in the data frame
 *
 * WARNING, YOU must ensure that brightness < 32
 * If not, undefined behaviour
 *
 * @param frame -- the data frame to be changed
 * @param brightness -- Brightness of the led
 */
void	spi_led_set_brightness(unsigned char *frame, uint16_t brightness) {
	// if (brightness > 31) {
	// 	frame[0] = 0xE0;
	// 	return ;
	// }
	frame[0] = 0xE0 | (0b00011111 & brightness); /* E0 is 11100000 (Mandatory)
												  * Then add brightness */
	return ;
}

/**
 * @brief Set led color in the data frame
 *
 * @param r -- Red color
 * @param g -- Green color
 * @color b == Blue color
 */
void	spi_led_set_color(unsigned char *frame, const uint8_t r,
		const uint8_t g, const uint8_t b) {
	frame[1] = b;
	frame[2] = g;
	frame[3] = r;
}

/**
 * @brief Set complete data frame for led 
 *
 * @param frame -- Data frame to update
 * @param brightness -- Led brightness value from 0 to 31
 * @param r -- Red color code
 * @param g -- Green color code
 * @param b -- Blue color code
 */
void	spi_led_set_data_frame(unsigned char *frame, const uint8_t brightness,
		const uint8_t r, const uint8_t g, const uint8_t b) {
	spi_led_set_brightness(frame, brightness);
	spi_led_set_color(frame, r, g, b);
}

/**
 * @brief Set the thrre RGB led by seeeting value directy
 *
 * VALUE are composed as follow:
 * [111] [Brightness:5bit] [RED] [GREEN] [BLUE]
 *
 * @param led_1 -- led 1 val
 * @param led_2 -- led 2 val
 * @param led_3 -- led 3 val
 */
void	spi_set_led(const uint32_t led_1, const uint32_t led_2, const uint32_t led_3) {
	unsigned char	buff_led_1[ADA102_DATA_SIZE];
	unsigned char	buff_led_2[ADA102_DATA_SIZE];
	unsigned char	buff_led_3[ADA102_DATA_SIZE];
	
	/* Set LED data frame */
	spi_led_set_data_frame(buff_led_1, 0xE0 | ((led_1 >> 24) & 0x1F),
		(led_1 >> 16) & 0xFF, (led_1 >> 8) & 0xFF, led_1 & 0xFF);
	spi_led_set_data_frame(buff_led_2, 0xE0 | ((led_2 >> 24) & 0x1F),
		(led_2 >> 16) & 0xFF, (led_2 >> 8) & 0xFF, led_2 & 0xFF);
	spi_led_set_data_frame(buff_led_3, 0xE0 | ((led_3 >> 24) & 0x1F),
		(led_3 >> 16) & 0xFF, (led_3 >> 8) & 0xFF, led_3 & 0xFF);

	/* Send SPI data */
	spi_led_send_start();
	spi_transmit_block(buff_led_1, 4);
	spi_transmit_block(buff_led_2, 4);
	spi_transmit_block(buff_led_3, 4);
	spi_led_send_stop();
}


int main() {
	spi_init_master();

	while (1) {
		spi_set_led(SPI_RED, SPI_OFF, SPI_OFF);
		_delay_ms(1000);
		spi_set_led(SPI_GREEN, SPI_OFF, SPI_OFF);
		_delay_ms(1000);
		spi_set_led(SPI_BLUE, SPI_OFF, SPI_OFF);
		_delay_ms(1000);
		spi_set_led(SPI_YELLOW, SPI_OFF, SPI_OFF);
		_delay_ms(1000);
		spi_set_led(SPI_CYAN, SPI_OFF, SPI_OFF);
		_delay_ms(1000);
		spi_set_led(SPI_MAGENTA, SPI_OFF, SPI_OFF);
		_delay_ms(1000);
		spi_set_led(SPI_WHITE, SPI_OFF, SPI_OFF);
		_delay_ms(1000);
	}
	
	return (0);
}
