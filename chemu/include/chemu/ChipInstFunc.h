#ifndef _CHIPINSTFUNC_H
#define _CHIPINSTFUNC_H

#include "ChipEmu.h"
#include "ChipInst.h"
#include "ChipAddress.h"

/* typedef for a function pointer
 */
typedef ChipInstResult (*ChipInstFunc)(ChipEmu*, ChipInst);

typedef enum {
	INST_CLASS_A,			// address class
	INST_CLASS_I,			// immediate class
	INST_CLASS_R,			// register class
	INST_CLASS_V			// void class
} ChipInstClass;

// // Address type instruction
// typedef ChipInstResult (*ChipInstFunc_A)(ChipEmu*, ChipAddress);
//
// // Draw type instruction
// typedef ChipInstResult (*ChipInstFunc_D)(ChipEmu*, int, int, int);
//
// // Immediate type instruction
// typedef ChipInstResult (*ChipInstFunc_I)(ChipEmu*, int, ChipReg);
//
// typedef ChipInstResult (*ChipInstFunc_R)(ChipEmu*, int, int);
//
// typedef ChipInstResult (*ChipInstFunc_V)(ChipEmu*);
//
// typedef union {
//     void *ptr;
//     ChipInstFunc_A atype;
//     ChipInstFunc_D dtype;
//     ChipInstFunc_I itype;
//     ChipInstFunc_R rtype;
//     ChipInstFunc_V vtype;
// } ChipInstFuncUnion;
//
// typedef struct {
//     ChipInstFuncUnion func;
//     ChipInstClass cls;
// } ChipInstFunc;

#endif
