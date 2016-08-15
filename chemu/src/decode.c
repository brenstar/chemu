#include "chemu/decode.h"

#include "chemu/instructions.h"
#include <stdlib.h>

const ChipOp CHIP_OPTABLE[] = {
    {.func = cif_sys,   .cls = INST_CLASS_A, .name="sys"},
    {.func = cif_cls,   .cls = INST_CLASS_V, .name="cls"},
    {.func = cif_ret,	.cls = INST_CLASS_V, .name="ret"},
    {.func = cif_j,     .cls = INST_CLASS_A, .name="j"},
    {.func = cif_call,  .cls = INST_CLASS_A, .name="call"},
    {.func = cif_sei,   .cls = INST_CLASS_I, .name="sei"},
    {.func = cif_sni,   .cls = INST_CLASS_I, .name="sni"},
    {.func = cif_se,    .cls = INST_CLASS_R, .name="se"},
    {.func = cif_li,    .cls = INST_CLASS_I, .name="li"},
    {.func = cif_addi,  .cls = INST_CLASS_I, .name="addi"},
    {.func = cif_move,  .cls = INST_CLASS_R, .name="move"},
    {.func = cif_or,    .cls = INST_CLASS_R, .name="or"},
    {.func = cif_and,   .cls = INST_CLASS_R, .name="and"},
    {.func = cif_xor,   .cls = INST_CLASS_R, .name="xor"},
    {.func = cif_add,   .cls = INST_CLASS_R, .name="add"},
    {.func = cif_sub,   .cls = INST_CLASS_R, .name="sub"},
    {.func = cif_shr,   .cls = INST_CLASS_R, .name="shr"},
    {.func = cif_subn,  .cls = INST_CLASS_R, .name="subn"},
    {.func = cif_shl,   .cls = INST_CLASS_R, .name="shl"},
    {.func = cif_sn,    .cls = INST_CLASS_R, .name="sn"},
    {.func = cif_la,    .cls = INST_CLASS_A, .name="la"},
    {.func = cif_jo,    .cls = INST_CLASS_A, .name="jo"},
    {.func = cif_rnd,   .cls = INST_CLASS_I, .name="rnd"},
    {.func = cif_draw,  .cls = INST_CLASS_R, .name="draw"},
    {.func = cif_sip,   .cls = INST_CLASS_I, .name="sip"},
    {.func = cif_snip,  .cls = INST_CLASS_I, .name="snip"},
    {.func = cif_ld,    .cls = INST_CLASS_I, .name="ld"},
    {.func = cif_lk,    .cls = INST_CLASS_I, .name="lk"},
    {.func = cif_del,   .cls = INST_CLASS_I, .name="del"},
    {.func = cif_snd,   .cls = INST_CLASS_I, .name="snd"},
    {.func = cif_ii,    .cls = INST_CLASS_I, .name="ii"},
    {.func = cif_font,  .cls = INST_CLASS_I, .name="font"},
    {.func = cif_bcd,   .cls = INST_CLASS_I, .name="bcd"},
    {.func = cif_save,  .cls = INST_CLASS_I, .name="save"},
    {.func = cif_rest,  .cls = INST_CLASS_I, .name="rest"}
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



ChipInstDec chipdec_decode(ChipInst instruction, ChipInstClass cls) {
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


int chipdec_index(ChipInst instruction) {
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
