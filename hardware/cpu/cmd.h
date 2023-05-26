#ifndef CMD_H
#define CMD_H

#include <avr/io.h>

#define CLOCK_PIN  PB0
#define INPUT_PIN  PB1
#define OUTPUT_PIN PB2

void cmd_transmitByte(uint8_t data);
uint8_t cmd_receiveByte();
void cmd_process(uint8_t command)

#endif // CMD_H
