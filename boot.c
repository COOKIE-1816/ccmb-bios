#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdbool.h>

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

bool readBootlinkIni(char* bootlinkContent, uint16_t maxContentLength) {
    uint32_t sectorNumber = 0;
    uint8_t sectorData[512];
    uint16_t sectorOffset = 0;

    bool fileFound = false;

    while (!fileFound) {
        if (!sdcard_readSector(sectorNumber, sectorData))
            return false;

        if (sectorData[0] == 0x00)
            return false;

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

    if (file == NULL)
        return false;

    size_t bytesRead = fread(buffer, sizeof(uint8_t), bufferSize, file);
    fclose(file);

    if (bytesRead == 0)
        return false;

    return true;
}

void jumpToEntryPoint(void* entryPoint) {
    __builtin_avr_delay_cycles(0);
    asm volatile("jmp %0" ::"r"(entryPoint));
}

bool _bootFromFile(const char* bootFilename) {
    const size_t bufferSize = 4096;
    uint8_t buffer[bufferSize];

    if (!loadFile(bootFilename, buffer, bufferSize))
        return false;

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
            // Handle the error appropriately
        }

        _bootFromFile(bootFilename);
        return true;
    }
}

bool bootFromDefault() {
    _bootFromFile("BOOT.BIN")
    return true;
}

void boot() {
    if (bootFromFile())
        return;

    if (bootFromDefault())
        return;

    // TODO: Handle the error
}
