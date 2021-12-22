#include "board.h"
#include <xc.h>

// Ugly macro expansion stuff needed so we can #define the LED pins
#define _X_PORT(X) __X_PORT(X)
#define _X_LAT(X) __X_LAT(X)
#define _X_TRIS(X) __X_TRIS(X)

#define __X_PORT(X) PORT ## X // token concatenation operator
#define __X_LAT(X) LAT ## X 
#define __X_TRIS(X) TRIS ## X

void gpio_init(void) {
    _X_TRIS(RED_LED_PIN) = 0;     // set as output
    _X_LAT(RED_LED_PIN) = !LED_ON;      // turn the led off

    _X_TRIS(BLUE_LED_PIN) = 0;     // set as output
    _X_LAT(BLUE_LED_PIN) = !LED_ON;      // turn the led off

    _X_TRIS(WHITE_LED_PIN) = 0;     // set as output
    _X_LAT(WHITE_LED_PIN) = !LED_ON;      // turn the led off
}

void RED_LED_SET(bool value) {
    _X_LAT(RED_LED_PIN) = !value ^ LED_ON;
}
void BLUE_LED_SET(bool value) {
    _X_LAT(BLUE_LED_PIN) = !value ^ LED_ON;
}
void WHITE_LED_SET(bool value) {
    _X_LAT(WHITE_LED_PIN) = !value ^ LED_ON;
}
