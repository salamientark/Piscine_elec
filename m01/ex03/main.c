/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/05 15:25:29 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define STEP 0x061A
#define MAX_DUTY_CYCLE 0x3D08
/**
 * Volatile variables are variables that may changed at any moment.
 * Like at interuption pin signal... So the compiler will not try to optimize it
 * This is particularly usefull when you want to be sure a variable will not 
 * change during compilation or else

 *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf/
 *  Configuring the PIN Page 12
 *  And Timer1 register datas are found from page 120-145
 *  Register description can be found at pages 100-101
 */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	DDRB |= (1 << DDB1); /* Set PB1 (LED1) pin as output */

	DDRD &= ~((1 << DDD2) | (1 << DDD4)); /* Set the button 1 on 2 as outputs */
	// DDRD &= ~(1 << DDD2);

	TCCR1A = 0B10000010; /* Clear OC1A on compare match
						  * Set OC1A at bottom
						  * Set mode to fast PWM
						  * Make the led blink
						  */
	TCCR1B = 0B00011101; /* Set Wave Generation Mode to FastPWM
						 * Set prescaler to 1024
						 */
	OCR1AH = 0x06; /* Set the high byte of the output compare register */
	OCR1AL = 0x1A; /* Set the low byte of the output compare register */

	// Define counter TOP value
	ICR1H = 0x3D; /// Set the high byte of the input capture register
	ICR1L = 0x08; /// Set the low byte of the input capture register
}

/**
* @brief Get the state of a button
*
* @param pin The pin to read
* @return The state of the button
*/
static uint8_t get_button_state(uint8_t pin) {
	return (PIND & (1 << pin)); /// Get button state on pin pin
}

/**
* @brief Increment the duty cycle of the PWM
*/
static void increment_duty_cycle(void) {
	uint16_t current_duty_cycle = (OCR1AH << 8) + OCR1AL;
	if (current_duty_cycle + (STEP << 1) > MAX_DUTY_CYCLE) {
		OCR1AH = MAX_DUTY_CYCLE >> 8; /// Change OUTPUT_COMAPRE_REGISTER high value
		OCR1AL = MAX_DUTY_CYCLE & 0xFF; /// Change OUTPUT_COMPARE_REGISTER low value
		return ;
	}
	OCR1AH += (STEP >> 8); /// Change OUTPUT_COMAPRE_REGISTER high value
	OCR1AL += (STEP & 0xFF); /// Change OUTPUT_COMPARE_REGISTER low value

}

/**
* @brief Decrement the duty cycle of the PWM
*/
static void decrement_duty_cycle(void) {
	uint16_t current_duty_cycle = (OCR1AH << 8) + OCR1AL;
	if (current_duty_cycle < (STEP << 1)) {
		OCR1AH = 0x06; /// Set the high byte of the output compare register
		OCR1AL = 0x1A; /// Set the low byte of the output compare register
		return ;
	}
	OCR1AH -= (STEP >> 8); /// Change OUTPUT_COMAPRE_REGISTER high value
	OCR1AL -= (STEP & 0xFF); /// Change OUTPUT_COMPARE_REGISTER low value
}

int main() {
	uint8_t	b1_last_state = 1;
	uint8_t	b2_last_state = 1;
	uint8_t b1_state;
	uint8_t b2_state;

	init();
	while (1) {
		b1_state = get_button_state(PD2);
		b2_state = get_button_state(PD4);

		if (b1_state == 0 && b1_last_state == 1){ /// Pressed button
			_delay_ms(20);
			b1_last_state = 0;
		}
		if (b2_state == 0) { /// Pressed Button
			_delay_ms(20);
			b2_last_state = 0;
		}
	
		if (b1_state && b1_last_state == 0) {
			_delay_ms(20);
			increment_duty_cycle();
			b1_last_state = 1;
		}
		if (b2_state && b2_last_state == 0) {
			_delay_ms(20);
			decrement_duty_cycle();
			b2_last_state = 1;
		}
	}
}
