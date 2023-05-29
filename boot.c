#define F_CPU 16000000UL

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "boot.h"
#include "hardware/sdcard/sdcard.h"
#include "hardware/sram/sram.h"
#include "hardware/cpu/cpu.h"
#include "hardware/leds/leds.h"
#include "error/error.h"

/*#ifndef F_CPU
    #error "CPU frequency not defined."
#endif*/

volatile uint8_t ledState;

/*
MOVED THIS TO SDCARD.C

bool fileExist(const char* filename) {
    char buff[13];
    
    if (!sdcard_dir(0))
        return false;

    while (sdcard_nextFile(buff, sizeof(buff), NULL)) {
        if (strcmp(buff, filename) == 0)
            return true;
    }

    return false;
}*/

bool readBootlinkIni(char* bootlinkContent, uint16_t maxContentLength) {
    uint32_t sectorNumber = 0;
    uint8_t sectorData[512];
    uint16_t sectorOffset = 0;

    bool fileFound = false;

    while (!fileFound) {
        if (!sdcard_readSector(sectorNumber, sectorData)){
            error(ERR_BOOT_UNREADABLE);
            return false;
        }

        if (sectorData[0] == 0x00) {
            error(ERR_BOOT_UNREADABLE);
            return false;
        }

        if (sectorData[0] != 0xE5 && (sectorData[11] & 0x08) == 0) {
            char filename[13];

            sdcard_currentFilename(sectorData, sectorOffset, filename);

            if (strcmp(filename, "BOOTLINK.INI") == 0) {
                uint16_t contentOffset = 0;
                uint16_t contentLength = sectorData[28] + (sectorData[29] << 8);

                while (contentLength > 0 && contentOffset < maxContentLength) {
                    bootlinkContent[contentOffset++] = sectorData[32 + contentOffset];
                    contentLength--;
                }

                bootlinkContent[contentOffset] = '\0';
                fileFound = true;
            }
        }

        sectorOffset += 32;
        if (sectorOffset >= 512) {
            sectorNumber++;
            sectorOffset = 0;
        }
    }

    return true;
}

bool loadFile(const char* filename, uint8_t* buffer, size_t bufferSize) {
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        error(ERR_BOOT_UNREADABLE);
        return false;
    }

    size_t bytesRead = fread(buffer, sizeof(uint8_t), bufferSize, file);
    fclose(file);

    if (bytesRead == 0) {
        //error(ERR_BOOT_UNREADABLE);
        return false;
    }

    return true;
}

void jumpToEntryPoint(void* entryPoint) {
    //__builtin_avr_delay_cycles(0);
    //asm volatile("jmp %0" ::"r"(entryPoint));

    cli();

    asm volatile (
        "jmp 0x0000"
    );
}

bool _bootFromFile(const char* bootFilename) {
    const size_t bufferSize = 4096;
    uint8_t buffer[bufferSize];

    if (!loadFile(bootFilename, buffer, bufferSize)){
        error(ERR_BOOT_UNREADABLE);
        return false;
    }

    jumpToEntryPoint(buffer);
    return true;
}

bool bootFromFile() {
    char bootlinkContent[MAX_BOOTLINK_SIZE];

    if (readBootlinkIni(bootlinkContent, MAX_BOOTLINK_SIZE)) {
        char bootFilename[32];
        char* newlinePos = strchr(bootlinkContent, '\n');

        if (newlinePos != NULL) {
            size_t bootFilenameLength = newlinePos - bootlinkContent;

            strncpy(bootFilename, bootlinkContent, bootFilenameLength);
            bootFilename[bootFilenameLength] = '\0';  // Null-terminate the bootFilename string
        } else {
            error(ERR_BOOT_NOTFOUND);
            return false;
        }

        _bootFromFile(bootFilename);
        return true;
    }
}

bool bootFromDefault() {
    _bootFromFile("BOOT.BIN");
    return true;
}

ISR(TIMER1_COMPA_vect) {
    ledState ^= (1 << PB5);

    if (ledState) {
        //PORTB |= (1 << PB5);
        red(0xff);
    } else {
        //PORTB &= ~(1 << PB5);
        red(0x00);
    }
}

void boot() {
    DDRB |= (1 << PB5);

    TCCR1B |= (1 << WGM12);
    OCR1A = 15624; // Set compare value for 2 Hz frequency (16 MHz clock)
    TIMSK1 |= (1 << OCIE1A);

    sei();

    TCCR1B |= (1 << CS12) | (1 << CS10);

    if (bootFromFile() || bootFromDefault()) {
        TCCR1B &= ~((1 << CS12) | (1 << CS10));
        PORTB &= ~(1 << PB5);

        cpu_on();

        return;
    }

    if (bootFromDefault())
        return;

    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    PORTB &= ~(1 << PB5);

    error(ERR_BOOT);
    //return;
}
