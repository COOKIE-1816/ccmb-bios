#ifndef SPI_H
#define SPI_H

#define SPI_MOSI_PIN PB3
#define SPI_MISO_PIN PB4
#define SPI_SCK_PIN PB5

void spi_init(void);
uint8_t spi_transfer(uint8_t data);

#endif
