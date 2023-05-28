#ifndef CPU_H
#define CPU_H

#include <avr/io.h>

#define CPU_VCC PD0

#define CPU_FAN PC3

void cpu_off();
void cpu_on();
void cpu_reset();

#endif
