#ifndef ERRDEF_H
#define ERRDEF_H

#define ERR_BOOT                                    0x01
#define ERR_BOOT_INEXISTENT                         0x02
#define ERR_BOOT_UNREADABLE                         0x03
#define ERR_BOOT_NOTFOUND                           0x04
#define ERR_SDCARD_NORESPONSE                       0x05
#define ERR_SDCARD_INIT                             0x06
#define ERR_SDCARD_BADRESPONSE                      0x07
#define ERR_SDCARD_PARTUNREADABLE                   0x08
#define ERR_SDCARD_BLOCKUNREADABLE                  0x09
#define ERR_SDCARD_TIMEOUT                          0x0a
#define ERR_SDCARD_UNHANDLEDRESPONSEEXPECTION       0x0b
#define ERR_SDCARD_UNSUPPORTEDFS                    0x0c
#define ERR_SDCARD_ANALYZEERROR                     0x0d
#define ERR_SDCARD_NOROOT                           0x0e
#define ERR_SDCARD_NULLSECTOR                       0x0f
#define ERR_SDCARD_UNSKIPABLE                       0x10
#define ERR_FILE_NULL                               0x11
#define ERR_FILE_BADMODE                            0x12
#define ERR_FILE_BADNAME                            0x13
#define ERR_FILE_WRITEERROR                         0x14


#endif // ERRDEF_H
