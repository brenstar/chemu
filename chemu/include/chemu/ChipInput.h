#ifndef _CHIPINPUT_H
#define _CHIPINPUT_H

#include <stdint.h>

// Enumeration of keys of the CHIP-8 keyboard
typedef enum ChipKey_e {
    CHIP_KEY_0 = 0x0,
    CHIP_KEY_1 = 0x1,
    CHIP_KEY_2 = 0x2,
    CHIP_KEY_3 = 0x3,
    CHIP_KEY_4 = 0x4,
    CHIP_KEY_5 = 0x5,
    CHIP_KEY_6 = 0x6,
    CHIP_KEY_7 = 0x7,
    CHIP_KEY_8 = 0x8,
    CHIP_KEY_9 = 0x9,
    CHIP_KEY_A = 0xA,
    CHIP_KEY_B = 0xB,
    CHIP_KEY_C = 0xC,
    CHIP_KEY_D = 0xD,
    CHIP_KEY_E = 0xE,
    CHIP_KEY_F = 0xF
} ChipKey;

// Enum of all possible states for a ChipKey, released or pressed
typedef enum ChipKeyState_e {
    CHIP_KEYSTATE_RELEASED = 0,
    CHIP_KEYSTATE_PRESSED = 1
} ChipKeyState;

// Type definition containing the state of the CHIP-8 keyboard. A 16-bit
// integer is used to store the state of each key. Each bit represents a
// key, the least significant bit is CHIP_KEY_0 and the most significant
// bit is CHIP_KEY_F.
typedef uint16_t ChipInput;


#endif
