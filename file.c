#define F_CPU 16000000UL

#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "hardware/sdcard/sdcard.h"
#include "error/error.h"
