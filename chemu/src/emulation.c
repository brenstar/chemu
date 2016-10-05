#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "threads.h"

#include "chemu/emulation.h"
#include "chemu/decode.h"
#include "chemu/display.h"
#include "chemu/input.h"
#include "chemu/memory.h"
#include "chemu/stack.h"
#include "chemu/timer.h"
#include "chemu/logger.h"

#include "chemu/ChipGetKeyCallback.h"
#include "chemu/ChipRedrawCallback.h"
#include "chemu/ChipEmuState.h"


static ChipInstResult __execute(ChipEmu emu, ChipInst inst);

static void __stop(ChipEmu emu);

static int __run(void* arg);

struct ChipEmu_s {
    ChipMem memory;
    ChipEmuState state;
    int limiter;
    // mutex for synchronizing any call to a monitor function
    // must be locked at start of function, then unlocked before return
    mtx_t emulatorLock;
    // mutex for synchronizing any access to the emulator's memory
    mtx_t memlock;
    cnd_t getKeyCV;
    ChipGetKeyCallback getKeyCb;
    ChipRedrawCallback redrawCb;
};

//
// Monitor: YES
//
ChipEmuState chipemu_break(ChipEmu emu) {
    return CHIPEMU_IDLE;
}

ChipEmu chipemu_create(void) {

    ChipEmu emu = (ChipEmu)malloc(sizeof(struct ChipEmu_s));
    if (emu != NULL) {
        // set callbacks to NULL
        emu->getKeyCb = NULL;
        emu->redrawCb = NULL;

        emu->state = CHIPEMU_READY;

        mtx_init(&emu->emulatorLock, mtx_plain);
        mtx_init(&emu->memlock, mtx_plain);

        chipmem_init(&emu->memory);
        chipemu_reset(emu);
    }

    return emu;
}

//
// Monitor: YES
//
ChipEmuState chipemu_continue(ChipEmu emu) {
    return CHIPEMU_LOOP;
}

//
// Monitor: YES
//
void chipemu_destroy(ChipEmu emu) {
    mtx_lock(&emu->emulatorLock);
    if (LOOPACTIVE(emu->state))
        __stop(emu);
    mtx_unlock(&emu->emulatorLock);

    mtx_destroy(&emu->emulatorLock);
    mtx_destroy(&emu->memlock);

    free(emu);
}

//
// Monitor: YES
//
ChipInstResult chipemu_execute(ChipEmu emu, ChipInst inst) {
    mtx_lock(&emu->emulatorLock);

    mtx_lock(&emu->memlock);
    ChipInstResult res = __execute(emu, inst);
    mtx_unlock(&emu->memlock);

    mtx_unlock(&emu->emulatorLock);

    return res;
}

//
// Monitor: NO
//
ChipKey chipemu_getKey(ChipEmu emu) {
    if (emu->getKeyCb != NULL)
        return emu->getKeyCb(emu);
    return CHIP_KEY_0;
}

//
// Monitor: NO
//
void chipemu_getDisplay(ChipEmu emu, ChipDisplay *displayDest) {
    mtx_lock(&emu->memlock);
    *displayDest = emu->memory.reserved.display;
    mtx_unlock(&emu->memlock);
}

//
// Monitor: NO
//
void chipemu_getStack(ChipEmu emu, ChipStack *stackDest) {
    mtx_lock(&emu->memlock);
    *stackDest = emu->memory.reserved.stack;
    mtx_unlock(&emu->memlock);
}

//
// Monitor: NO
//
void chipemu_getDatapath(ChipEmu emu, ChipDP *datapathDest) {
    mtx_lock(&emu->memlock);

    datapathDest->pc = emu->memory.reserved.pc;
    datapathDest->addrReg = emu->memory.reserved.addrReg;
    // copy registers
    memcpy(datapathDest->regs, emu->memory.reserved.regs, CHIP_REG_COUNT);
    datapathDest->delTimer = emu->memory.reserved.delTimer;
    datapathDest->sndTimer = emu->memory.reserved.sndTimer;

    mtx_unlock(&emu->memlock);
}
//
// Monitor: NO
// Reads the binary file stored in path and copies the read data into
// the emulator's memory struct. CHIP_LOAD_FAILURE is returned if the
// file could not be opened, a read error occurs, or the ROM is too big (the
// max number of bytes was read and EOF was not reached).
//
int chipemu_loadROM(ChipEmu emu, const char *path) {
    int bytesRead = CHIP_LOAD_FAILURE;
    FILE *fp;
    #ifdef _WIN32
        fopen_s(&fp, path, "rb");
    #else
        fp = fopen(path, "rb");
    #endif
    if (fp != NULL) {
        uint8_t buffer[CHIPMEM_DATA_LEN];
        bytesRead = fread(buffer, 1, CHIPMEM_DATA_LEN, fp);
        if (bytesRead == CHIPMEM_DATA_LEN && !feof(fp))
            bytesRead = CHIP_LOAD_FAILURE; // ROM file was too big
        else {
            mtx_lock(&emu->memlock);
            memcpy(emu->memory.array + CHIP_PRGM_START, buffer, bytesRead);
            mtx_unlock(&emu->memlock);
        }
        fclose(fp);
    }
    return bytesRead;
}

int chipemu_mainLoop(ChipEmu emu) {

    int exitStatus = EXIT_SUCCESS;
    return exitStatus;
}

void chipemu_setKey(ChipEmu emu, ChipKey key, ChipKeyState state) {
    mtx_lock(&emu->memlock);
    chipin_set(&emu->memory.reserved.input, key, state);
    mtx_unlock(&emu->memlock);

    if (state == CHIP_KEYSTATE_PRESSED) {
        // wake up emulator thread if needed
    }
}

//
// Monitor: YES
//
ChipEmuState chipemu_start(ChipEmu emu) {
    mtx_lock(&emu->emulatorLock);

    if (emu->state == CHIPEMU_READY) {
        // create thread

    }

    mtx_unlock(&emu->emulatorLock);
    return emu->state;
}

//
// Monitor: YES
//
ChipEmuState chipemu_stop(ChipEmu emu) {
    return CHIPEMU_READY;
}

int chipemu_step(ChipEmu emu) {
    int result = CHIP_STEP_SUCCESS;

    mtx_lock(&emu->emulatorLock);

    if (emu->state == CHIPEMU_ERROR) {
        // do nothing, error occurred previously
        mtx_unlock(&emu->emulatorLock);
        return CHIP_STEP_FAILURE;
    }

    mtx_lock(&emu->memlock);

    // fetch
    ChipAddress pc = emu->memory.reserved.pc;
    ChipInst instruction = (uint16_t)(emu->memory.array[pc] << 8 |
                                      emu->memory.array[pc + 1]);

    switch (__execute(emu, instruction)) {
        case INST_SUCCESS:
            break;
        case INST_SUCCESS_INCR_PC:
            emu->memory.reserved.pc += 2;
            break;
        case INST_FAILURE:
            //chiplog_error("Instruction %d:%s has failed.\n", i, op.name);
            result = CHIP_STEP_FAILURE;
            break;
    }

    mtx_unlock(&emu->memlock);
    mtx_unlock(&emu->emulatorLock);

    return result;
}


void chipemu_redraw(ChipEmu emu) {
    if (emu->redrawCb != NULL)
        emu->redrawCb(emu);
}

ChipEmuState chipemu_reset(ChipEmu emu) {

    mtx_lock(&emu->emulatorLock);

    if (LOOPACTIVE(emu->state))
        __stop(emu);

    emu->state = CHIPEMU_READY;

    mtx_lock(&emu->memlock);
    // clear datapath
    emu->memory.reserved.pc = CHIP_PRGM_START;
    emu->memory.reserved.addrReg = 0;
    for (int i = 0; i < 16; ++i)
        emu->memory.reserved.regs[i] = 0;
    emu->memory.reserved.sndTimer = 0;
    emu->memory.reserved.delTimer = 0;

    // clear stack
    chipstack_init(&emu->memory.reserved.stack);

    // clear input
    emu->memory.reserved.input = 0;

    // clear framebuffer
    chipdisplay_clear(&emu->memory.reserved.display);

    mtx_unlock(&emu->memlock);
    mtx_unlock(&emu->emulatorLock);
}

// Static helper functions
//-----------------------------------------------------------------------------

// NOT thread-safe, mutexes must be locked before calling this function
ChipInstResult __execute(ChipEmu emu, ChipInst inst) {
    ChipInstResult result;

    // decode
    int i = chipdec_index(inst);
    if (i == NO_INSTRUCTION) {
        chiplog_error("Illegal instruction: %04X\n", inst);
        result = INST_FAILURE;
    } else {
        ChipOp op = CHIP_OPTABLE[i];
        ChipInstDec decoded = chipdec_decode(inst, op.cls);

        // execute
        chiplog_debug("Executing '%s' (0x%04X)\n", op.name, inst);
        result = op.func(emu, &emu->memory, decoded);
    }

    if (result == INST_FAILURE)
        emu->state = CHIPEMU_ERROR;

    return result;
}

void __stop(ChipEmu emu) {

}
