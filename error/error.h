#ifndef ERROR_H
#define ERROR_H
#include "errdef.h"

#define SHIFT_DATA_PIN   PB6
#define SHIFT_CLOCK_PIN  PC6
#define SHIFT_LATCH_PIN  4

void error(int err);

#endif // ERROR_H
