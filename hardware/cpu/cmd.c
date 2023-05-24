#include <avr/io.h>
#include <util/delay.h>
#include "cmd.h"

void cmd_transmitByte(uint8_t data) {
    PORTB &= ~(1 << OUTPUT_PIN);
    _delay_us(10);

    for(uint8_t i = 0; i < 8; i++) {
        if(data & (1 << i)) {
            PORTB |= (1 << OUTPUT_PIN);
        } else {
            PORTB |= ~(1 << OUTPUT_PIN);
        }

        while(!(PINB & (1 << CLOCK_PIN)));
        while(PINB & (1 << CLOCK_PIN));
    }

    PORTB |= (1 << OUTPUT_PIN);
    _delay_us(10);
}

uint8_t cmd_receiveByte() {
    uint8_t data = 0;

    while(PINB & (1 << INPUT_PIN));
    for(uint8_t i = 0; i < 8; i++) {
        while(!(PINB & (1 << CLOCK_PIN))) {}

        if((PINB & (1 << INPUT_PIN)) {
           data |= (1 << i);
        }

        while(PINB & (1 << CLOCK_PIN));
    }

    return data;
}

//void cmd_process(uint8_t command) __attribute__ {}
