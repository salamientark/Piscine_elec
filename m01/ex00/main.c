/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/03 18:53:20 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/**
 * Volatile variables are variables that may changed at any moment.
 * Like at interuption pin signal... So the compiler will not try to optimize it
 * This is particularly usefull when you want to be sure a variable will not 
 * change during compilation or else
 */

static void wait(volatile uint64_t time) {
	while (time--) {}
}

int main() {
	DDRB |= (1 << DDB1); /* We are setting the data_direction register to output
						  * for the first LED
						  */
	while (1) {
		PORTB ^= (1 << PB1); /* The ^ (XOR) operator perform an exclusive or operation
							  * 0 ^ 1 = 1 only. 0 ^ 0 = 0, 1 ^ 1 = 0
							  * So it will revert only the right bit
							  */
		wait(80000);
	}
}
