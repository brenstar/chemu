
#include "chemu/input.h"
#include "chemu/ChipInput.h"


ChipKeyState chemu_in_keystate(ChipInput *input, ChipKey key) {
    return ((*input >> (int)key) & 1) == 1 ? CHIP_KEYSTATE_PRESSED
                                           : CHIP_KEYSTATE_RELEASED;
}

void chemu_in_set(ChipInput *input, ChipKey key, ChipKeyState state) {
    switch (state) {
        case CHIP_KEYSTATE_RELEASED:
            *input &= ~(1 << (int)key);      // clear bit at key
            break;
        case CHIP_KEYSTATE_PRESSED:
            *input |= 1 << (int)key;         // set bit at key
            break;
    }
}
