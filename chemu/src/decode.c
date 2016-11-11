#include "chemu/decode.h"

#include "chemu/instructions.h"
#include <stdlib.h>

const ChipOp CHEMU_OPTABLE[] = {
    {.func = chemu_if_sys,   .cls = INST_CLASS_A, .name="sys"},
    {.func = chemu_if_cls,   .cls = INST_CLASS_V, .name="cls"},
    {.func = chemu_if_ret,   .cls = INST_CLASS_V, .name="ret"},
    {.func = chemu_if_j,     .cls = INST_CLASS_A, .name="j"},
    {.func = chemu_if_call,  .cls = INST_CLASS_A, .name="call"},
    {.func = chemu_if_sei,   .cls = INST_CLASS_I, .name="sei"},
    {.func = chemu_if_sni,   .cls = INST_CLASS_I, .name="sni"},
    {.func = chemu_if_se,    .cls = INST_CLASS_R, .name="se"},
    {.func = chemu_if_li,    .cls = INST_CLASS_I, .name="li"},
    {.func = chemu_if_addi,  .cls = INST_CLASS_I, .name="addi"},
    {.func = chemu_if_move,  .cls = INST_CLASS_R, .name="move"},
    {.func = chemu_if_or,    .cls = INST_CLASS_R, .name="or"},
    {.func = chemu_if_and,   .cls = INST_CLASS_R, .name="and"},
    {.func = chemu_if_xor,   .cls = INST_CLASS_R, .name="xor"},
    {.func = chemu_if_add,   .cls = INST_CLASS_R, .name="add"},
    {.func = chemu_if_sub,   .cls = INST_CLASS_R, .name="sub"},
    {.func = chemu_if_shr,   .cls = INST_CLASS_R, .name="shr"},
    {.func = chemu_if_subn,  .cls = INST_CLASS_R, .name="subn"},
    {.func = chemu_if_shl,   .cls = INST_CLASS_R, .name="shl"},
    {.func = chemu_if_sn,    .cls = INST_CLASS_R, .name="sn"},
    {.func = chemu_if_la,    .cls = INST_CLASS_A, .name="la"},
    {.func = chemu_if_jo,    .cls = INST_CLASS_A, .name="jo"},
    {.func = chemu_if_rnd,   .cls = INST_CLASS_I, .name="rnd"},
    {.func = chemu_if_draw,  .cls = INST_CLASS_R, .name="draw"},
    {.func = chemu_if_sip,   .cls = INST_CLASS_I, .name="sip"},
    {.func = chemu_if_snip,  .cls = INST_CLASS_I, .name="snip"},
    {.func = chemu_if_ld,    .cls = INST_CLASS_I, .name="ld"},
    {.func = chemu_if_lk,    .cls = INST_CLASS_I, .name="lk"},
    {.func = chemu_if_del,   .cls = INST_CLASS_I, .name="del"},
    {.func = chemu_if_snd,   .cls = INST_CLASS_I, .name="snd"},
    {.func = chemu_if_ii,    .cls = INST_CLASS_I, .name="ii"},
    {.func = chemu_if_font,  .cls = INST_CLASS_I, .name="font"},
    {.func = chemu_if_bcd,   .cls = INST_CLASS_I, .name="bcd"},
    {.func = chemu_if_save,  .cls = INST_CLASS_I, .name="save"},
    {.func = chemu_if_rest,  .cls = INST_CLASS_I, .name="rest"}
};


// decoder function, takes an instruction and returns an offset
typedef int (*DecoderFunc)(ChipInst);

static int decoder0(ChipInst inst);
static int decoder5(ChipInst inst);
static int decoder8(ChipInst inst);
static int decoder9(ChipInst inst);
static int decoderE(ChipInst inst);
static int decoderF(ChipInst inst);


typedef struct {
    int index;
    DecoderFunc func;
} Decoder;

static const Decoder DEC_TABLE[] = {
    {0, decoder0},      // sys, cls, ret
    {3, NULL},          // j
    {4, NULL},          // call
    {5, NULL},          // sei
    {6, NULL},          // sni
    {7, decoder5},      // se
    {8, NULL},          // li
    {9, NULL},          // addi
    {10, decoder8},     // move, or, and, xor, add, sub, shr, subn, shl
    {19, decoder9},     // sn
    {20, NULL},         // la
    {21, NULL},         // jo
    {22, NULL},         // rnd
    {23, NULL},         // draw
    {24, decoderE},     // sip, snip
    {26, decoderF},     // ld, lk, del, snd, ii, font, bcd, save, rest
};



ChipInstDec chemu_dec_decode(ChipInst instruction, ChipInstClass cls) {
    ChipInstDec decoded;
    switch (cls) {
        case INST_CLASS_A:
            decoded.a.literal = nibble4(instruction);
            decoded.a.addr = addr(instruction);
            break;
        case INST_CLASS_I:
            decoded.i.literal = nibble4(instruction);
            decoded.i.rnum = nibble3(instruction);
            decoded.i.immediate = lowerbyte(instruction);
            break;
        case INST_CLASS_R:
            decoded.r.literal_hi = nibble4(instruction);
            decoded.r.rnum_dest = nibble3(instruction);
            decoded.r.rnum_src = nibble2(instruction);
            decoded.r.literal_lo = nibble1(instruction);
            break;
        case INST_CLASS_V:
            decoded.instruction = instruction;
            break;
    }
    return decoded;
}


int chemu_dec_index(ChipInst instruction) {
    int i = nibble4(instruction);
    Decoder dec = DEC_TABLE[i];
    int index = dec.index;

    if (dec.func != NULL) {
        int offset = dec.func(instruction);
        if (offset == NO_INSTRUCTION)
            return NO_INSTRUCTION;
        index += offset;
    }

    return index;
}

static int decoder0(ChipInst inst) {
    switch (inst) {
        case 0x00E0:
            return 1;
        case 0x00EE:
            return 2;
        default:
            return 0;
    }
}

static int decoder5(ChipInst inst) {
    return nibble1(inst) == 0 ? 0 : NO_INSTRUCTION;
}

static int decoder8(ChipInst inst) {
    int offset = inst & 0xF;
    if (offset > 7 && offset != 0xE)
        return NO_INSTRUCTION;
    return offset == 0xE ? 8 : offset;
}

static int decoder9(ChipInst inst) {
    return nibble1(inst) == 0 ? 0 : NO_INSTRUCTION;
}

static int decoderE(ChipInst inst) {
    switch (inst & 0xFF) {
        case 0x9E:
            return 0;
        case 0xA1:
            return 1;
        default:
            return NO_INSTRUCTION;
    }
}

static int decoderF(ChipInst inst) {
    switch (inst & 0xFF) {
        case 0x07:
            return 0;
        case 0x0A:
            return 1;
        case 0x15:
            return 2;
        case 0x18:
            return 3;
        case 0x1E:
            return 4;
        case 0x29:
            return 5;
        case 0x33:
            return 6;
        case 0x55:
            return 7;
        case 0x65:
            return 8;
        default:
            return NO_INSTRUCTION;
    }
}
