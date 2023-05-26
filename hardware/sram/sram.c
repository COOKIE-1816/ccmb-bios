#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include "sram.h"
#include "spi.h"
#include <avr/io.h>

void sram_init(void) {
  DDRB |= (1 << SRAM_CS_PIN);

  spi_init();

  // TODO: Configure SPI settings for SRAM communication
  // TODO: Set SPI mode, clock frequency, and other relevant settings

  PORTB &= ~(1 << SRAM_CS_PIN);
  PORTB |= (1 << SRAM_CS_PIN);
}

void sram_write_byte(uint16_t address, uint8_t data) {
    PORTB &= ~(1 << SRAM_CS_PIN);

    spi_transfer(SRAM_WRITE);
    spi_transfer((address >> 8) & 0xFF);
    spi_transfer(address & 0xFF);

    spi_transfer(data);

    PORTB |= (1 << SRAM_CS_PIN);
}

uint8_t sram_readByte(uint16_t address) {
    uint8_t data;

    PORTB &= ~(1 << SRAM_CS_PIN);

    spi_transfer(SRAM_READ);
    spi_transfer((address >> 8) & 0xFF);
    spi_transfer(address & 0xFF);

    data = spi_transfer(0xFF);

    PORTB |= (1 << SRAM_CS_PIN);

    return data;
}

void sram_writeBlock(uint16_t address, const uint8_t* data, uint16_t length) {
    sram_select();

    spi_transfer(SRAM_WRITE);
    spi_transfer((address >> 8) & 0xFF);
    spi_transfer(address & 0xFF);

    for (uint16_t i = 0; i < length; i++) {
        spi_transfer(data[i]);
    }

    PORTB |= (1 << SRAM_CS_PIN);
    sram_deselect();
}

void sram_readBlock(uint16_t startAddress, uint8_t* buffer, uint16_t length) {
    sram_select();

    spi_transfer(SRAM_READ);
    spi_transfer((uint8_t)(startAddress >> 8));
    spi_transfer((uint8_t)(startAddress & 0xFF));

    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = spi_transfer(0x00);
    }

    sram_deselect();
}

void sram_select() {
    PORTB &= ~(1 << SRAM_SS_PIN);
}

void sram_deselect() {
    PORTB |= (1 << SRAM_SS_PIN);
}
