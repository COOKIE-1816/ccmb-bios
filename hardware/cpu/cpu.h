#ifndef CPU_H
#define CPU_H

#include <avr/io.h>

#define CPU_VCC PD0

void cpu_off();
void cpu_on();

#endif
