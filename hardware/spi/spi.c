#include "spi.h"

void spi_init(void) {
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
    DDRB &= ~(1 << DDB4);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);

    // Set SPI mode (optional)
    //SPCR |= (1 << CPOL) | (1 << CPHA);

    // Set clock frequency (optional)
    //SPCR |= (1 << SPR1) | (1 << SPR0);
    //SPSR |= (1 << SPI2X);

    //#define SPI_INIT_COMPLETE
}

uint8_t spi_transfer(uint8_t data) {
  SPDR = data;

  while (!(SPSR & (1 << SPIF)));
  return SPDR;
}
