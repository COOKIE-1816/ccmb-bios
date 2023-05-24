#ifndef COMMANDS_H
#define COMMANDS_H

//      COMMAND              BINARY        HEX  DEC  Direction
// -------------------------------------------------------------
#define BIOS_CMD_RX_SHUTDOWN 0b00000001 // 0x01 1    rx
#define BIOS_CMD_RX_RESET    0b00000010 // 0x02 2    rx
#define BIOS_CMD_RX_SLEEP    0b00000100 // 0x03 3    rx
#define BIOS_CMD_RX_WAKE     0b00001000 // 0x04 4    rx
#define BIOS_CMD_RX_HALT     0b00010000 // 0x05 5    rx
#define BIOS_CMD_RX_NEXT     0b00100000 // 0x06 6    rx
#define BIOS_CMD_RX_LEDS_OFF 0b01000000 // 0x07 7    rx
#define BIOS_CMD_RX_LEDS_R   0b10000000 // 0x08 8    rx
#define BIOS_CMD_RX_LEDS_Y   0b00000011 // 0x09 9    rx
#define BIOS_CMD_RX_LEDS_G   0b00000101 // 0x0a 10   rx
#define BIOS_CMD_TX_ERROR    0b00000110 // 0x0b 11   tx
#define BIOS_CMD_TX_FATAL    0b00000111 // 0x0c 12   tx
#define BIOS_CMD_RX_PING     0b00001001 // 0x0d 13   rx
#define BIOS_CMD_TX_PING     0b00001010 // 0x0e 14   tx
#define BIOS_CMD_RX_MEMTEST  0b00001100 // 0x0f 15   rx
#define BIOS_CMD_TX_MEMERR   0b00001011 // 0x10 16   tx
#define BIOS_CMD_RX_FAN_ON   0b00001101 // 0x11 17   rx
#define BIOS_CMD_RX_FAN_OFF  0b00001111 // 0x12 18   rx
#define BIOS_CMD_RX_BEEP     0b00010001 // 0x13 19   rx

#endif
