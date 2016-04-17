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
	ChipStack stack;
    //ChipMem memory;
} * ChipEmu;

typedef int (*ChipInstFunc)(ChipEmu, uint16_t);

#endif
