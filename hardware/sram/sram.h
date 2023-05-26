#ifndef SRAM_H
#define SRAM_H

#define SRAM_SS_PIN PB0
#define SRAM_CS_PIN PB6

/*
#define SRAM_WRITE_COMMAND 0x02
#define SRAM_READ_COMMAND 0x03
*/

#define SRAM_READ  0b00000011
#define SRAM_WRITE 0b00000010
#define SRAM_RDSR  0b00000101
#define SRAM_WRSR  0b00000001

#define SRAM_BASE_ADDRESS 0x00000000
#define SRAM_SIZE 10240

void sram_init(void);
void sram_select();
void sram_deselect();
void sram_writeByte(uint16_t address, uint8_t data);
uint8_t sram_readByte(uint16_t address);
void sram_readBlock(uint16_t startAddress, uint8_t* buffer, uint16_t length);
void sram_writeBlock(uint16_t address, const uint8_t* data, uint16_t length)

#endif
