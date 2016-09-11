#ifndef _CHIPADDRESS_H
#define _CHIPADDRESS_H

#include <stdint.h>

//
// Type definition for a memory address in the CHIP-8 emulator. This type is
// guarenteed to be an unsigned integer and is at least 12 bits wide. Valid
// addresses range from 0 to 4095 (0xFFF)
//
typedef uint16_t ChipAddress;


#endif
