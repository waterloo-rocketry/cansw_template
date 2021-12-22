#ifndef BOARD_H
#define	BOARD_H

#include <stdbool.h>

#define MAX_LOOP_TIME_DIFF_ms 250

#define RED_LED_PIN C2
#define BLUE_LED_PIN C3
#define WHITE_LED_PIN C4
#define LED_ON 0 // Whether the pin going high or low turns on the LED.

void gpio_init(void);

void RED_LED_SET(bool value);
void BLUE_LED_SET(bool value);
void WHITE_LED_SET(bool value);

#endif	/* BOARD_H */

