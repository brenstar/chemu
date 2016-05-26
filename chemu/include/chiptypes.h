#ifndef _CHIPTYPES_H
#define _CHIPTYPES_H

#include <stdint.h>

#define CHIP_STACK_SIZE 16

#define REGISTERS 16

#define CHIP_MEM_LEN 4096

#define CARRY_REG 15

#define ILLEGAL_INST 0xFFFF

// data memory locations

#define CHIP_PRGM_START 0x200
#define CHIP_ETIPRGM_START 0x600
#define CHIP_END 0xFFF

typedef struct ChipStackStruct {
    uint16_t data[CHIP_STACK_SIZE];
    uint8_t index;
} * ChipStack;

typedef struct ChipEmuStruct {
	uint16_t pc;
//    uint16_t *sp;
//    uint16_t stack[CHIP_STACK_SIZE];
    uint16_t addressReg;
    uint8_t registers[REGISTERS];
    uint8_t sndTimer;
    uint8_t delayTimer;
    uint8_t memory[CHIP_MEM_LEN];
    uint16_t keymap;
	ChipStack stack;
    //ChipMem memory;
} * ChipEmu;

typedef union {

	uint16_t instruction;

	struct {
		uint16_t addr:12;
		uint16_t reserved:4;
	} atype;

	struct {
		uint16_t immediate:8;
		uint16_t rnum:4;
		uint16_t reserved:4;
	} itype;

	struct {
		uint16_t reserved_lo:4;
		uint16_t ra_num:4;       // aux register number
		uint16_t rs_num:4;       // source register number
		uint16_t reserved_hi:4;
	} rtype;

	struct {
		uint16_t rows:4;
		uint16_t ry_num:4;
		uint16_t rx_num:4;
		uint16_t reserved:4;
	} dtype;

} ChipInst;

typedef int (*ChipInstFunc)(ChipEmu, ChipInst);

#endif
