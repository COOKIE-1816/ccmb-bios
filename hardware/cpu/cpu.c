#define F_CPU 16000000UL

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include "cpu.h"
#include "../../boot.h"
#include "../sram/sram.h"
#include <avr/io.h>
#include <util/delay.h>

void cpu_off() {
    PORTB &= ~(CPU_VCC << 1);
}

void cpu_on() {
    PORTB |= (CPU_VCC << 1);
}

void cpu_reset() {
    cpu_off();
    _delay_ms(20);

    volatile uint8_t* sram = (volatile uint8_t*)SRAM_BASE_ADDRESS;
        for (uint32_t i = 0; i < SRAM_SIZE /*10240*/; i++) {
            *sram = 0;
            sram++;
        }
    
    boot();
    //cpu_on();
}