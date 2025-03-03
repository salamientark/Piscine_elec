/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/03 19:20:30 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/**
 * Volatile variables are variables that may changed at any moment.
 * Like at interuption pin signal... So the compiler will not try to optimize it
 * This is particularly usefull when you want to be sure a variable will not 
 * change during compilation or else
 */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	DDRB |= (1 << PB1);
}


int main() {
	init();

	while (1) {
		PORTB ^= (1 << PB1);
	}
}
