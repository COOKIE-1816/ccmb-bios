#ifndef SDCARD_H
#define SDCARD_H

#include <avr/io.h>
#include <stdint.h>

#define SD_CS_PIN 2
#define SD_MOSI_PIN 3
#define SD_MISO_PIN 4
#define SD_SCK_PIN 5

#define DATA_START_TOKEN 0xFE
#define DATA_ACCEPTED 0x05

#define CMD_GO_IDLE_STATE 0x40
#define CMD_SEND_IF_COND 0x48
#define CMD_READ_SINGLE_BLOCK 0x51
#define CMD_READ_MULTIPLE_BLOCKS 0x52
#define CMD_WRITE_SINGLE_BLOCK 0x58
#define CMD_WRITE_MULTIPLE_BLOCKS 0x59
#define CMD_STOP_TRANSMISSION 0x4C
#define CMD_SET_BLOCK_LENGTH 0x50
#define CMD_APP_CMD 0x77

#define ACMD_SEND_OP_COND 0x29
#define ACMD_SET_WR_BLK_ERASE_COUNT 0x17

#define MAX_FILENAME_LENGTH 12
#define SECTOR_SIZE 512

uint8_t sdcard_command(uint8_t cmd uint32_t arg);
uint8_t sdcard_init(void);
uint8_t sdcard_readPart(uint8_t partition);
uint8_t sdcard_readBlock(uint8_t *buffer);
uint8_t sdcard_readSector(uint32_t sector, uint8_t* buffer);
bool getFs(uint8_t partitionType, uint32_t* rootDirectorySector, uint8_t* fileSystemType);
bool analyzeSector(uint8_t* sectorData, uint32_t* rootDirectorySector);
bool sdcard_root();
bool sdcard_dir(uint8_t partition);
bool sdcard_skipFile(uint8_t* sectorData, uint16_t* sectorOffset);
bool sdcard_nextFile(uint8_t* sectorData, uint16_t* sectorOffset, char* filename);

#endif
