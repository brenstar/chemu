#include "chemu/decode.h"

#include "chemu/instructions.h"

#define op(name, cls) {(void*)cif_ ## name, INST_CLASS_ ## cls}

// const ChipInstFunc CHIP_OPTABLE[] = {
//     {cif_sys,   INST_CLASS_A},
//     {cif_cls,   INST_CLASS_V},
//     {cif_ret,   INST_CLASS_V},
//     {cif_j,     INST_CLASS_A},
//     {cif_call,  INST_CLASS_A},
//     {cif_sei,   INST_CLASS_I},
//     {cif_sni,   INST_CLASS_I},
//     {cif_se,    INST_CLASS_R},
//     {cif_li,    INST_CLASS_I},
//     {cif_addi,  INST_CLASS_I},
//     {cif_move,  INST_CLASS_R},
//     {cif_or,    INST_CLASS_R},
//     {cif_and,   INST_CLASS_R},
//     {cif_xor,   INST_CLASS_R},
//     {cif_add,   INST_CLASS_R},
//     {cif_sub,   INST_CLASS_R},
//     {cif_shr,   INST_CLASS_R},
//     {cif_subn,  INST_CLASS_R},
//     {cif_shl,   INST_CLASS_R}
//
// };

const ChipInstFunc CHIP_OPTABLE[] = {
    op(sys, A),
    op(cls, V),
    op(ret, V),
    op(j, A),

}

ChipInstFunc chipdec_decode(ChipInst instruction) {
    ChipInstFunc f = op(sys, V);

    return f;
}
