#define F_CPU 16000000UL

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "cmd.h"
#include "commands.h"
#include "cpu.h"
#include "../leds/leds.h"
#include "../../sleep.h"

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

        if((PINB & (1 << INPUT_PIN))) {
           data |= (1 << i);
        }

        while(PINB & (1 << CLOCK_PIN));
    }

    return data;
}

void cmd_process(uint8_t command) {
    switch (command) {
        case BIOS_CMD_RX_SHUTDOWN:
            cpu_off();
            break;
        
        case BIOS_CMD_RX_RESET:
            cpu_reset();
            break;

        case BIOS_CMD_RX_SLEEP:
            sleep();
            break;

        /*case BIOS_CMD_RX_HALT:
            while(1);
            
            break;*/


        case BIOS_CMD_RX_LEDS_OFF:
            led_off();
            break;
        
        case BIOS_CMD_RX_LEDS_R:
            red(0xFF);
            break;
        
        case BIOS_CMD_RX_LEDS_Y:
            yellow(0xFF);
            break;
        
        case BIOS_CMD_RX_LEDS_G:
            green(0xFF);
            break;

        case BIOS_CMD_RX_PING:
            cmd_transmitByte(BIOS_CMD_TX_PING);
            break;
        
        case BIOS_CMD_RX_MEMTEST:
            // TODO: Memory test
            break;

        case BIOS_CMD_RX_FAN_ON:
            PORTC |= (1 << CPU_FAN);
            break;

        case BIOS_CMD_RX_FAN_OFF:
            PORTC &= ~(1 << CPU_FAN);
            break;

        case BIOS_CMD_RX_BEEP:
            PORTC |= (1 << PC4);
            _delay_ms(200);
            PORTC &= ~(1 << PC4);

            break;
        
        default:
            // Command unknown or unsupported.
            cmd_transmitByte(BIOS_CMD_TX_ERROR);

            led_off();
            red(0xFF);

            delay_ms(200);
            led_off();

            break;
    }
}