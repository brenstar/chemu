#ifndef _CHIPINPUT_H
#define _CHIPINPUT_H

#include <stdint.h>

typedef enum ChipKey_e {
    CHIP_KEY_0,
    CHIP_KEY_1,
    CHIP_KEY_2,
    CHIP_KEY_3,
    CHIP_KEY_4,
    CHIP_KEY_5,
    CHIP_KEY_6,
    CHIP_KEY_7,
    CHIP_KEY_8,
    CHIP_KEY_9,
    CHIP_KEY_A,
    CHIP_KEY_B,
    CHIP_KEY_C,
    CHIP_KEY_D,
    CHIP_KEY_E,
    CHIP_KEY_F
} ChipKey;

typedef enum ChipKeyState_e {
    CHIP_KEYSTATE_RELEASED,
    CHIP_KEYSTATE_PRESSED
} ChipKeyState;

// 16 bit integer, each bit representing the state of the key
typedef uint16_t ChipInput;


#endif
