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
#include "error/error.h"

typedef struct {
    const char* filename;
    const char* mode;
    uint8_t* buffer;
    size_t bufferSize;
    size_t bytesRead;
} FILE;

bool fopen(FILE* file, const char* filename, const char* mode) {
    if (file != NULL){
        error(ERR_FILE_NULL);
        return false;
    }
    
    file->filename = filename;
    file->mode = mode;
    file->buffer = NULL;
    file->bufferSize = 0;
    file->bytesRead = 0;

    if (strcmp(mode, "r") != 0) {
        error(ERR_FILE_BADMODE);
        return NULL;
    }

    if (filename == NULL || filename[0] == '\0') {
        error(ERR_FILE_BADNAME);
        return NULL;
    }

    if (!sdcard_init()) {
        return NULL;
    }
    
    size_t filenameLength = strlen(filename);
    if (filenameLength == 0 || filenameLength >= MAX_FILENAME_LENGTH) {
        error(ERR_FILE_BADNAME);
        return NULL;
    }
    
    uint8_t response = sdcard_sendCommand(0x11, blockNumber);
    if (response != 0x00) {
        error(ERR_SDCARD_NORESPONSE);
        return NULL;
    }

    while (spi_receive() != 0xFE);
    
    for (uint16_t i = 0; i < 512; i++) {
        file->buffer[i] = spi_receive();
    }

    // Implement the necessary logic to open the file
    // For example, you would need to interface with the underlying hardware
    // and file system to open the file
    
    // Here, you can implement your own logic to handle file opening
    // using the specific hardware and file system you are working with
    
    // Placeholder code, replace with actual implementation
    
    // Create a FILE structure to represent the opened file
    FILE* file = (FILE*)malloc(sizeof(FILE));
    
    // Perform the necessary operations to open the file
    // For example, send commands to the hardware or file system
    
    return file;

    //return true;
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