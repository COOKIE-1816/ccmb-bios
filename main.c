#define F_CPU 16000000UL

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>
//#include "hardware/analog.h" - deprecated
#include "boot.h"
#include "hardware/cpu/cmd.h"
#include "hardware/cpu/cpu.h"
#include "hardware/leds/leds.h"
#include "hardware/sdcard/sdcard.h"
#include "hardware/sram/sram.h"
#include "error/error.h"

#define BIOS_ADDRESS 0x0000

void jumpToBootloader();
void jumpToBootloader() {
    void (*bootloader)(void) = (void(*)(void)) 0x7000;

    bootloader();
}

int main(void) {
    // Set pin modes as output or as an input. This is really important.
    DDRB &= ~(1 << CLOCK_PIN);          // PB0  0   00
    DDRB &= ~(1 << INPUT_PIN);          // PB1  1   00
    DDRB |=  (1 << SD_CS_PIN);          // PB2  2   ff
    DDRB |=  (1 << SD_MOSI_PIN);        // PB3  3   ff
    DDRB |=  (1 << SHIFT_LATCH_PIN);    // PB4  4   ff
    DDRB |=  (1 << SD_SCK_PIN);         // PB5  5   ff
    DDRB |=  (1 << SHIFT_DATA_PIN);     // PB6  6   ff
    DDRB |=  (1 << SRAM_SS_PIN);        // PB7  7   ff
    DDRC |=  (1 << LED_RED);            // PC0  0   ff
    DDRC |=  (1 << LED_YELLOW);         // PC1  1   ff
    DDRC |=  (1 << LED_GREEN);          // PC2  2   ff
    DDRC |=  (1 << CPU_FAN);            // PC3  3   ff
    DDRC |=  (1 << SRAM_CS_PIN);        // PC4  4   ff
    DDRC |=  (1 << OUTPUT_PIN);         // PC5  5   ff
    DDRC |=  (1 << SHIFT_CLOCK_PIN);    // PC6  6   ff
    DDRD |=  (1 << CPU_VCC);            // PD0  0   ff
    DDRD &= ~(1 << SD_MISO_PIN);        // PD1  1   00

    PORTB |= (1 << INPUT_PIN);

    boot();

    
    uint8_t receivedByte;

    while (1) {
        receivedByte = cmd_receiveByte();
        
        if (receivedByte != 0x00)
            cmd_process(receivedByte);
        
        /*if (terminate)
            break;*/
    }

    return 0;
}
