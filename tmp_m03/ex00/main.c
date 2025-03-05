/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */ /*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:31:16 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/04 22:58:14 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/** *All the information that I needed for this can be found at:
 *  - https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 */

/* ************************************************************************** */
/*                                   INIT                                     */
/* ************************************************************************** */

/**
 * @brief Initialize DATA_DIRECTION registers
 */
static void init(void) {
	/* TIMER10 Setup */
	TCCR1A = 0b00000000; /* Set CTC mode */
	TCCR1B = 0b00001101; /* Set prescaler to 1024 */
	OCR1A = (uint16_t) (F_CPU / 1024 - 1); /* Set compare value */
	TIMSK1 = 0b00000010; /* Enable interrupt when comapre with OCCR0A occur */
	
	/* RGB LED INIT */
	DDRD = 0b01101000; /// Set RGB LED as outputs in the DATA_DIRECTION_REGISTER
	PORTD |= (1 << PD5); /* Set RED LED to RED */
}

/* ************************************************************************** */
/*                                 INTERUPTS                                  */
/* ************************************************************************** */
ISR(TIMER1_COMPA_vect) {
	if (PORTD & (1 << PD3)){ /* LED ON BLUE */
		PORTD &= ~(1 << PD3); /* LED OFF BLUE */
		PORTD |= (1 << PD5); /* LED ON RED */
	}	
	else if (PORTD & (1 << PD5)){ /* LED ON RED */
		PORTD &= ~(1 << PD5); /* LED OFF RED */
		PORTD |= (1 << PD6); /* LED ON GREEN */
	}
	else if (PORTD & (1 << PD6)){ /* LED ON GREEN */
		PORTD &= ~(1 << PD6); /* LED OFF GREEN */
		PORTD |= (1 << PD3); /* LED ON BLUE */
	}
}

/* ************************************************************************** */
/*                                    MAIN                                    */
/* ************************************************************************** */

int main() {
	init();

	sei(); /* Enable global interrupts */

	while (1) {}
	return (0);
}
