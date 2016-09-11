#ifndef _CHIPOP_H
#define _CHIPOP_H

#include "ChipInstFunc.h"

// Type definition for a Chip operation structure. Chip operation structures
// contain a function pointer to a Chip instruction function, a class and a
// 4 character string representing the mnemonic
typedef struct ChipOp_s {
    ChipInstFunc func;
    ChipInstClass cls;
    const char name[5]; // mnemonic
} ChipOp;

#endif
