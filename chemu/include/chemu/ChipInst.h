#ifndef _CHIPINST_H
#define _CHIPINST_H

#include <stdint.h>

#include "ChipEmu.h"



typedef enum ChipInstResult_e {
	INST_SUCCESS,			// successful execution
	INST_SUCCESS_INCR_PC,	// successful execution, increment program counter
	INST_FAILURE			// failed exectuion
} ChipInstResult;

typedef uint16_t ChipInst;

// typedef enum {
// 	INST_CLASS_A,			// address class
// 	INST_CLASS_D,			// draw class
// 	INST_CLASS_I,			// immediate class
// 	INST_CLASS_R,			// register class
// 	INST_CLASS_V,			// void class
// 	INST_CLASS_FUNC			// function class
// } ChipInstClass;
//
// typedef struct ChipInst_AType_s {
// 	uint16_t addr:12;
// 	uint16_t reserved:4;
// } ChipInst_AType;
//
// typedef struct ChipInst_IType_s {
// 	uint16_t immediate:8;
// 	uint16_t rnum:4;
// 	uint16_t reserved:4;
// } ChipInst_IType;
//
// typedef struct ChipInst_RType_s {
// 	uint16_t reserved_lo:4;
// 	uint16_t ra_num:4;			// aux register number
// 	uint16_t rs_num:4;			// source register number
// 	uint16_t reserved_hi:4;
// } ChipInst_RType;
//
// typedef struct ChipInst_DType_s {
// 	uint16_t rows:4;			// number of rows of sprite
// 	uint16_t ry_num:4;			// register containing x coordinate
// 	uint16_t rx_num:4;			// register containing y coordinate
// 	uint16_t reserved:4;
// } ChipInst_DType;
//
// /* Typedef representing a CHIP-8 instruction. A CHIP-8 instruction is
//  * 2 bytes long and stored big endian. Instructions have 4 classes: atype,
//  * itype, rtype and dtype.
//  */
// typedef union {
//
// 	uint16_t instruction;
// 	ChipInst_AType atype;
// 	ChipInst_IType itype;
// 	ChipInst_RType rtype;
// 	ChipInst_DType dtype;
//
// } ChipInst;

/* typedef for a function pointer
 */
//typedef ChipInstResult (*ChipInstFunc)(ChipEmu*, ChipInst);

// typedef int (*ChipInstFunc_AType)(ChipEmu*, uint16_t);
//
// typedef int (*ChipInstFunc_IType)(ChipEmu*, ChipReg, uint8_t);
//
// typedef int (*ChipInstFunc_RType)(ChipEmu*, ChipReg, ChipReg);
//
// typedef int (*ChipInstFunc_DType)(ChipEmu*, )

#endif
