#include "leds.h"

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>

void red(uint8_t v) {
    if(v == 0xFF){
        PORTC |= (1 << LED_RED);
        return;
    }

    PORTC &= ~(1 << LED_RED);
}

void yellow(uint8_t v) {
    if(v == 0xFF){
        PORTC |= (1 << LED_YELLOW);
        return;
    }

    PORTC &= ~(1 << LED_YELLOW);
}

void green(uint8_t v) {
    if(v == 0xFF){
        PORTC |= (1 << LED_GREEN);
        return;
    }

    PORTC &= ~(1 << LED_GREEN);
}

void led_off() {
    red(0x00);
    yellow(0x00);
    green(0x00);
}