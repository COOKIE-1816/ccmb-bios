#ifndef FAT32FS_H
#define FAT32FS_H

#include <stdint.h>

typedef struct {
    uint8_t jmpInstruction[3];
    char oemName[8];
} BootSector;

typedef struct {
    uint32_t entryValue;
} FATEntry;

typedef struct {
    FATEntry* entries;
} FATTable;

typedef struct {
    char fileName[11];
    uint8_t attributes;
} DirectoryEntry;

typedef struct {
    DirectoryEntry entry;
    
    uint32_t currentCluster;
    uint32_t currentPosition;
} FCB;

#endif