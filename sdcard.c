#include "sdcard.h"
#include "spi.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SDCARD_DISABLE() PORTB |= (1 << SD_CS_PIN);

uint8_t sdcard_command(uint8_t cmd uint32_t arg) {
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
        return 0;
    }

    if (!sdcard_wait(DATA_START_TOKEN)) {
        return 0;
    }

  return 1; // Return 1 for success, or 0 for failure
}

uint8_t sdcard_readPart(uint8_t partition) {
  PORTB &= ~(1 << SD_CS_PIN);

    if (!sdcard_command(CMD_READ_PARTITION, partition)) {
        return 0;
    }

    if (!sdcard_wait(DATA_START_TOKEN)) {
        // Handle response error
        return 0;
    }

    uint8_t buffer[512];

    for (uint16_t i = 0; i < 512; i++) {
        buffer[i] = spi_transfer(0xFF);
    }

    PORTB |= (1 << SD_CS_PIN);

    return 1; // Return 1 for success, or 0 for failure
}

uint8_t sdcard_readBlock(uint32_t blockNum, uint8_t *buffer) {
    uint32_t start_addr = block_num * SD_BLOCK_SIZE;

    PORTB &= ~(1 << SD_CS_PIN);

    if (!sdcard_command(CMD_READ_SINGLE_BLOCK, start_addr)) {
        return 0;
    }

    if (!sdcard_wait(DATA_START_TOKEN)) {
        return 0;
    }

    for (uint16_t i = 0; i < SD_BLOCK_SIZE; i++) {
        buffer[i] = spi_transfer(0xFF);
    }

    PORTB |= (1 << SD_CS_PIN);

    return 1;
}

uint8_t sdcard_wait(uint8_t expected_response) {
    uint16_t timeout = 0;

    while (spi_transfer(0xFF) != expected_response) {
        timeout++;

        if (timeout > SD_TIMEOUT) {
            // Handle timeout error
            return 0;
        }
    }

    return 1; // Return 1 for success, or 0 for failure
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
            return false;
    }
}

bool analyzeSector(uint8_t* sectorData, uint32_t* rootDirectorySector) {
    if (sectorData[510] != 0x55 || sectorData[511] != 0xAA)
        return false;

    for (uint8_t entryOffset = 0; entryOffset < 16; entryOffset += 16) {
        uint8_t partitionType = sectorData[entryOffset + 4];

        if (partitionType == 0x01 || partitionType == 0x04 || partitionType == 0x06 || partitionType == 0x0E) {
            uint32_t startSector = *((uint32_t*)&sectorData[entryOffset + 8]);

            if(!getFs())
                return false

            *rootDirectorySector = startSector;

            return true;
        }
    }

    return false;
}

bool sdcard_root() {
    uint32_t partition_sector = 0;
    uint8_t buffer[512];

    if (!sdcard_readSector(partition_sector, buffer))
        return false;

    if (!analyzeSector(sectorData, &rootDirectorySector))
        return false;


    return true; // Return true if successful, false otherwise
}

bool sdcard_dir(uint8_t partition) {
    uint32_t sector = 0;
    uint8_t buffer[512];

    if (!sdcard_readSector(sector, buffer)) {
        return false;
    }

    return sdcard_root();

    //return true;
}


bool sdcard_skipFile(uint8_t* sectorData, uint16_t* sectorOffset) {
    *sectorOffset += 32;

    if (*sectorOffset >= SECTOR_SIZE) {
        uint32_t nextSector = currentSector + 1;

        if (!sdcard_readSector(nextSector, sectorData))
            return false;

        *sectorOffset = 0;

        if (sectorData[0] == 0x00)
            return false;
    }

    return true;
}

bool sdcard_nextFile(uint8_t* sectorData, uint16_t* sectorOffset, char* filename) {
    while (true) {
        uint8_t attributes = sectorData[*sectorOffset + 11];

        if (attributes != 0x0F && attributes != 0x08 && attributes != 0x10) {
            if (!sdcard_skipFile(sectorData, sectorOffset))
                return false;

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
