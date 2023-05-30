#define F_CPU 16000000UL

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include "sdcard.h"
#include "../spi/spi.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include "../../error/error.h"

#define SDCARD_DISABLE() PORTB |= (1 << SD_CS_PIN);

uint32_t startSector;
uint32_t sectorsPerFat;
uint8_t numberOfFats;
uint32_t sectorsPerFat32;
uint8_t numberOfFats32;

uint32_t* rootDirectorySector;
uint8_t* sectorData;

uint8_t sdcard_command(uint8_t cmd, uint32_t arg) {
    PORTB &= ~(1 << SD_CS_PIN);
    spi_transfer(cmd | 0x40);

    spi_transfer(arg >> 24);
    spi_transfer(arg >> 16);
    spi_transfer(arg >> 8);
    spi_transfer(arg);

    spi_transfer(0x95);

    uint8_t response = spi_transfer(0xFF);

    PORTB |= (1 << SD_CS_PIN);

    if ((response & 0x1F) != 0x01) {
        return 0;
    }

    while (spi_transfer(0xFF) == 0xFF) {}
    return 1;
}

uint8_t sdcard_init(void) {
    spi_init();

    DDRB |= (1 << SD_CS_PIN);

    _delay_ms(10);

    if (!sdcard_command(CMD_GO_IDLE_STATE, 0)) {
        error(ERR_SDCARD_NORESPONSE);
        return 0;
    }

    if (!sdcard_wait(DATA_START_TOKEN)) {
        error(ERR_SDCARD_NORESPONSE);
        return 0;
    }

  return 1;
}

uint8_t sdcard_wait(uint8_t expected_response) {
    uint16_t timeout = 0;

    while (spi_transfer(0xFF) != expected_response) {
        timeout++;

        if (timeout > SD_TIMEOUT) {
            error(ERR_SDCARD_TIMEOUT);
            return 0;
        }
    }

    return 1;
}

bool sdcard_readSector(uint32_t sector, uint8_t* buffer) {
    PORTB &= ~(1 << SD_CS_PIN);

    spi_transfer(0x40 | 17);

    spi_transfer(sector >> 24);
    spi_transfer(sector >> 16);
    spi_transfer(sector >> 8);
    spi_transfer(sector);

    spi_transfer(0x00);
    spi_transfer(0x00);

    while (spi_transfer(0xFF) != 0xFE);

    for (uint16_t i = 0; i < 512; i++) {
        buffer[i] = spi_transfer(0xFF);
    }

    spi_transfer(0xFF);
    spi_transfer(0xFF);

    PORTB |= (1 << SD_CS_PIN);

    return true;
}

bool getFs(uint8_t partitionType, uint32_t* rootDirectorySector, uint8_t* fileSystemType) {
    switch (partitionType) {
        case 0x01: // FAT12
        case 0x04: // FAT16 (partition smaller than 32MB)
        case 0x06: // FAT16 (partition larger  than 32MB)
            *fileSystemType = 0x01; // FAT file system
            *rootDirectorySector = startSector + sectorsPerFat * numberOfFats;

            return true;

        case 0x0B: // FAT32
        case 0x0C: // FAT32 (LBA)
            *fileSystemType = 0x01; // FAT file system
            *rootDirectorySector = startSector + sectorsPerFat32 * numberOfFats32;

            return true;

        default:
            error(ERR_SDCARD_UNSUPPORTEDFS);
            return false;
    }
}

bool analyzeSector(uint8_t* sectorData, uint32_t* rootDirectorySector) {
    if (sectorData[510] != 0x55 || sectorData[511] != 0xAA)
        {
            error(ERR_SDCARD_ANALYZEERROR);
            return false;
        }

    for (uint8_t entryOffset = 0; entryOffset < 16; entryOffset += 16) {
        uint8_t partitionType = sectorData[entryOffset + 4];

        if (partitionType == 0x01 || partitionType == 0x04 || partitionType == 0x06 || partitionType == 0x0E) {
            startSector = *((uint32_t*)&sectorData[entryOffset + 8]);

            if(!getFs(0x0c, rootDirectorySector, 0x01)) // TODO: fix this
                return false;

            rootDirectorySector = startSector;

            return true;
        }
    }

    return false;
}

bool sdcard_root() {
    uint32_t partition_sector = 0;
    uint8_t buffer[512];

    if (!sdcard_readSector(partition_sector, buffer)){
        error(ERR_SDCARD_NULLSECTOR);
        return false;
    }

    if (!analyzeSector(sectorData, &rootDirectorySector))
        //error(ERR_SDCARD_ANALYZEERROR); removed because it is useless
        return false;


    return true;
}

bool sdcard_dir(uint8_t partition) {
    uint32_t sector = 0;
    uint8_t buffer[512];

    if (!sdcard_readSector(sector, buffer)) {
        error(ERR_SDCARD_NULLSECTOR);
        return false;
    }

    return sdcard_root();

    //return true;
}


bool sdcard_skipFile(uint8_t* sectorData, uint16_t* sectorOffset) {
    uint32_t currentSector = 0;
    *sectorOffset += 32;

    if (*sectorOffset >= SECTOR_SIZE) {
        uint32_t nextSector = currentSector + 1;

        if (!sdcard_readSector(nextSector, sectorData)){
            error(ERR_SDCARD_UNSKIPABLE);
            return false;
        }

        *sectorOffset = 0;

        if (sectorData[0] == 0x00){
            error(ERR_SDCARD_NULLSECTOR);
            return false;
        }
    }

    return true;
}

bool sdcard_nextFile(uint8_t* sectorData, uint16_t* sectorOffset, char* filename) {
    while (true) {
        uint8_t attributes = sectorData[*sectorOffset + 11];

        if (attributes != 0x0F && attributes != 0x08 && attributes != 0x10) {
            if (!sdcard_skipFile(sectorData, sectorOffset)){
                
                return false;
            }

            continue;
        }

        memcpy(filename, sectorData + *sectorOffset, MAX_FILENAME_LENGTH);
        filename[MAX_FILENAME_LENGTH] = '\0';

        if (attributes == 0x08 || attributes == 0x10)
            return true;

        if (!sdcard_skipFile(sectorData, sectorOffset))
            return false;
    }
}

void sdcard_currentFilename(uint8_t* sectorData, uint16_t sectorOffset, char* filename) {
    memcpy(filename, sectorData + sectorOffset, MAX_FILENAME_LENGTH);
    filename[MAX_FILENAME_LENGTH] = '\0';
}

bool fileExist(const char* filename) {
    char buff[13];
    
    if (!sdcard_dir(0))
        return false;

    while (sdcard_nextFile(buff, sizeof(buff), NULL)) {
        if (strcmp(buff, filename) == 0)
            return true;
    }

    return false;
}
