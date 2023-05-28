#ifndef LEDS_H
#define LEDS_H

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define LED_RED PC0
#define LED_YELLOW PC1
#define LED_GREEN PC2

void red(uint8_t v);
void green(uint8_t v);
void yellow(uint8_t v);

void led_off();

#endif