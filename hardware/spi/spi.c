#ifndef __AVR_ATmega328P__
  #define __AVR_ATmega328P__
#endif

#include "spi.h"
#include <avr/io.h>
#include <string.h>
#include <stdint.h>

void spi_init(void) {
    DDRB |= (1 << PD3) | (1 << PD5) | (1 << PD2);
    DDRB &= ~(1 << PD4);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);

    SPCR &= ~(1 << CPOL);
    SPCR &= ~(1 << CPHA);
    
    SPCR &= ~(1 << SPR1);
    SPCR |= (1 << SPR0);
}

uint8_t spi_transfer(uint8_t data) {
  SPDR = data;

  while (!(SPSR & (1 << SPIF)));
  return SPDR;
}
