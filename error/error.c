#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include "error.h"

const uint8_t digitPatterns[] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111,  // 9
    0b01110111,  // A
    0b01111100,  // B
    0b00111001,  // C
    0b01011110,  // D
    0b01111001,  // E
    0b01110001   // F
};

void error(int err) {
    int digit1 = (err >> 4) & 0xF;
    int digit2 = err & 0xF;

    uint16_t data = (digitPatterns[digit1] << 8) | digitPatterns[digit2];

    for (int i = 0; i < 16; i++) {
        if (data & (1 << (15 - i))) {
            PORTB |= (1 << SHIFT_DATA_PIN);
        } else {
            PORTB &= ~(1 << SHIFT_DATA_PIN);
        }

        PORTB |= (1 << SHIFT_CLOCK_PIN);
        PORTB &= ~(1 << SHIFT_CLOCK_PIN);
    }

    PORTB |= (1 << SHIFT_LATCH_PIN);
    PORTB &= ~(1 << SHIFT_LATCH_PIN);
}
