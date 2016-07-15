#ifndef _CHIPINSTDEC_H
#define _CHIPINSTDEC_H

#include <stdint.h>
#include "ChipInst.h"

typedef struct {
    uint8_t literal;
    uint16_t addr;
} ChipInst_AType;

typedef struct {
    uint8_t literal;
    uint8_t rnum;
    uint8_t immediate;
} ChipInst_IType;

typedef struct {
    uint8_t literal_hi;
    uint8_t rnum_dest;
    uint8_t rnum_src;
    uint8_t literal_lo;
} ChipInst_RType;


typedef union {
    ChipInst instruction;
    ChipInst_AType a;
    ChipInst_IType i;
    ChipInst_RType r;
} ChipInstDec;



#endif
