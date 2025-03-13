/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:42:50 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/13 13:05:17 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Init button DATA_DIRECTION registers
 */
void	button_init(void) {
	/* Data Direction setup */
	DDRD &= ~((1 << BUTTON1) | (1 << BUTTON2)); /* Set buttons as inputs */
}

/**
 * @brief Read button status
 *
 * @param port -- Port register
 * @param pin -- Pin on the port
 * @return 0 if button is pressed, else 1
 */
uint8_t	read_button(const uint8_t port, const uint8_t pin) {
	return (port & (1 << pin));
}
