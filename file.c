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

#include "hardware/sdcard/sdcard.h"

typedef struct {
    const char* filename;
    const char* mode;
    uint8_t* buffer;
    size_t bufferSize;
    size_t bytesRead;
} FILE;

bool custom_fopen(FILE* file, const char* filename, const char* mode) {
    if (file != NULL)
        return false;
    
    file->filename = filename;
    file->mode = mode;
    file->buffer = NULL;
    file->bufferSize = 0;
    file->bytesRead = 0;

    
    
    // TODO: Implement file open logic

    return true;
}

size_t custom_fread(void* ptr, size_t size, size_t count, FILE* file) {
    // TODO: Implement file read logic specific to your system. read from the SD card or SPI communication
    // TODO: Return the number of bytes read
    
    return 0;
}

int custom_fclose(FILE* file) {
    // TODO: Implement file close logic specific to your system. close the SD card or SPI communication

    return 0;
}