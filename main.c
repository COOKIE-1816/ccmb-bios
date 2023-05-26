#define F_CPU 16000000UL

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include "hardware/analog.h"
#include "boot.h"
#include "hardware/cpu/cmd.h"

#define BIOS_ADDRESS 0x0000

void jumpToBootloader(void) {
    void (*bootloader)(void) = (void(*)(void)) 0x7000;
    bootloader();
}

int main(void) {
    DDRB |=  (1 << OUTPUT_PIN);
    DDRB &= ~(1 << INPUT_PIN);
    DDRB &= ~(1 << CLOCK_PIN);

    PORTB |= (1 << INPUT_PIN);

    while (1) {
        cmd_process(cmd_receiveByte());
    }

    return 0;
}
