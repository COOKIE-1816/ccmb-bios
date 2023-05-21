#ifndef CMD_H
#define CMD_H

#include <avr/io.h>

#define CLOCK_PIN  PB0
#define INPUT_PIN  PB1
#define OUTPUT_PIN PB2

void cmd_transmitByte(uint8_t data);
void cmd_receiveByte();

#endif // CMD_H
