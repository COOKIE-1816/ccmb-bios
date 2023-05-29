#include "sleep.h"
#include "hardware/cpu/cmd.h"
#include "hardware/cpu/commands.h"

void sleep() {
    int asleep;
    asleep = 1;

    while(asleep) {
        if(cmd_receiveByte() == BIOS_CMD_RX_WAKE)
            break;
    }
}