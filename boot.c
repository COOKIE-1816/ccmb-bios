#include <avr/io.h>
#include <avr/interrupt.h>

#include "boot.h"
#include "sdcard.h"
#include "sram.h"

#ifndef F_CPU
    #error "CPU frequency not defined."
#endif

#include <util/delay.h>

/*void boot() {
    MCUCR |= (1 << IVCE);
    MCUCR = (1 << IVSEL);

    sei(); // Enable global interrupts

    //if () {
        jumpToBootloader();
    //}

    if (!sdcard_init()) {
        while (1) {
            _delay_ms(500);
        }
    }

    if (!sdcard_readPart(0)) {
        while (1) {
            // TODO: Handle error condition
        }
    }

    sram_init();

    uint16_t address = 0x0000;
    uint8_t buffer[512];

    while (address < 0xFFFF) {
        if (!sdcard_readBlock(buffer)) {
            while (1) {
            }
        }

        sram_writeBlock(address, buffer);
        address += 512;
    }
}*/

bool fileExist(const char* file) {
    if (!sdcard_dir(0))
        return false;

    while (sdcard_nextFile()) {
        if (strcmp(sdcard_currentFilename(), filename) == 0)
            return true;
    }

    return false;
}
