#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "chemu/threads.h"

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


static ChipInstResult __execute(ChipEmu emu, ChipInst inst);

static void __updateTimer(ChipEmu emu, ChipReg *timerPtr);

struct ChipEmu_s {
    ChipMem memory;
    // mutex for synchronizing any access to the emulator's memory
    mtx_t memlock;
    mtx_t getKeyLock;
    cnd_t getKeyCV;
    int lastKeyPressed;
    // true if emulator is emulating a cycle, false otherwise
    bool stepping;
    ChipRedrawCallback redrawCb;
};


ChipEmu chemu_emu_create(void) {

    ChipEmu emu = (ChipEmu)malloc(sizeof(struct ChipEmu_s));
    if (emu != NULL) {
        // set callbacks to NULL
        emu->redrawCb = NULL;

        emu->stepping = false;

        mtx_init(&emu->memlock, mtx_plain);
        mtx_init(&emu->getKeyLock, mtx_plain);
        cnd_init(&emu->getKeyCV);

        chemu_mem_init(&emu->memory);
        chemu_emu_reset(emu);
    }

    return emu;
}

void chemu_emu_destroy(ChipEmu emu) {
    mtx_destroy(&emu->memlock);
    mtx_destroy(&emu->getKeyLock);
    cnd_destroy(&emu->getKeyCV);

    free(emu);
}

ChipInstResult chemu_emu_execute(ChipEmu emu, ChipInst inst) {

    mtx_lock(&emu->memlock);
    ChipInstResult res = __execute(emu, inst);
    mtx_unlock(&emu->memlock);

    return res;
}

ChipKey chemu_emu_getKey(ChipEmu emu) {
    //if (emu->getKeyCb != NULL)
    //    return emu->getKeyCb(emu);

    if (emu->stepping) {
        // no longer modifying memory, release lock
        mtx_unlock(&emu->memlock);

        mtx_lock(&emu->getKeyLock);
        emu->lastKeyPressed = -1;
        while (emu->lastKeyPressed == -1) {
            cnd_wait(&emu->getKeyCV, &emu->getKeyLock);
        }
        mtx_unlock(&emu->getKeyLock);

        mtx_lock(&emu->memlock);
    }

    return emu->lastKeyPressed;

}

void chemu_emu_getDisplay(ChipEmu emu, ChipDisplay *displayDest) {
    mtx_lock(&emu->memlock);
    //*displayDest = emu->memory.reserved.display;
    memcpy(displayDest, &(emu->memory.reserved.display), sizeof(ChipDisplay));
    mtx_unlock(&emu->memlock);
}

void chemu_emu_getStack(ChipEmu emu, ChipStack *stackDest) {
    mtx_lock(&emu->memlock);
    *stackDest = emu->memory.reserved.stack;
    mtx_unlock(&emu->memlock);
}

void chemu_emu_getDatapath(ChipEmu emu, ChipDP *datapathDest) {
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
// Reads the binary file stored in path and copies the read data into
// the emulator's memory struct. CHIP_LOAD_FAILURE is returned if the
// file could not be opened, a read error occurs, or the ROM is too big (the
// max number of bytes was read and EOF was not reached).
//
int chemu_emu_loadROM(ChipEmu emu, const char *path) {
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

void chemu_emu_setKey(ChipEmu emu, ChipKey key, ChipKeyState state) {
    if (state == CHIP_KEYSTATE_PRESSED) {
        // wake up emulator thread if needed
        mtx_lock(&emu->getKeyLock);
        if (emu->lastKeyPressed == -1) {
            emu->lastKeyPressed = key;
            chiplog_info("Waking up emulator thread with key press %d\n", key);
            cnd_signal(&emu->getKeyCV);
        }
        mtx_unlock(&emu->getKeyLock);
    }
    mtx_lock(&emu->memlock);
    chemu_in_set(&emu->memory.reserved.input, key, state);
    mtx_unlock(&emu->memlock);
}

void chemu_emu_setRedrawCallback(ChipEmu emu, ChipRedrawCallback callback) {
    emu->redrawCb = callback;
}

int chemu_emu_step(ChipEmu emu) {
    int result = CHIP_STEP_SUCCESS;

    mtx_lock(&emu->memlock);
    emu->stepping = true;

    // fetch
    ChipAddress pc = emu->memory.reserved.pc;
    ChipInst instruction = (uint16_t)(emu->memory.array[pc] << 8 |
                                      emu->memory.array[pc + 1]);

    switch (__execute(emu, instruction)) {
        case INST_SUCCESS:
            break;
        case INST_SUCCESS_INCR_PC:
            if (emu->memory.reserved.pc == 0xFFFF)
                emu->memory.reserved.pc = 0;
            else
                emu->memory.reserved.pc += 2;
            break;
        case INST_FAILURE:
            //chiplog_error("Instruction %d:%s has failed.\n", i, op.name);
            result = CHIP_STEP_FAILURE;
            break;
    }

    emu->stepping = false;
    mtx_unlock(&emu->memlock);
    return result;
}


void chemu_emu_redraw(ChipEmu emu) {
    if (emu->redrawCb != NULL && emu->stepping) {
        // release the memlock so the callback can access the display
        mtx_unlock(&emu->memlock);
        emu->redrawCb(emu);
        mtx_lock(&emu->memlock);
    }
}

void chemu_emu_reset(ChipEmu emu) {

    mtx_lock(&emu->memlock);
    // clear datapath
    emu->memory.reserved.pc = CHIP_PRGM_START;
    emu->memory.reserved.addrReg = 0;
    for (int i = 0; i < 16; ++i)
        emu->memory.reserved.regs[i] = 0;
    emu->memory.reserved.sndTimer = 0;
    emu->memory.reserved.delTimer = 0;

    // clear stack
    chemu_stack_init(&emu->memory.reserved.stack);

    // clear input
    emu->memory.reserved.input = 0;

    // clear framebuffer
    chemu_disp_clear(&emu->memory.reserved.display);

    mtx_unlock(&emu->memlock);
}

void chemu_emu_triggerDelayTimer(ChipEmu emu) {
    __updateTimer(emu, &emu->memory.reserved.delTimer);
}

void chemu_emu_triggerSoundTimer(ChipEmu emu) {
    __updateTimer(emu, &emu->memory.reserved.sndTimer);
}

// Static helper functions
//-----------------------------------------------------------------------------

// NOT thread-safe, mutexes must be locked before calling this function
ChipInstResult __execute(ChipEmu emu, ChipInst inst) {
    ChipInstResult result;

    // decode
    int i = chemu_dec_index(inst);
    if (i == NO_INSTRUCTION) {
        chiplog_error("Illegal instruction: %04X\n", inst);
        result = INST_FAILURE;
    } else {
        ChipOp op = CHEMU_OPTABLE[i];
        ChipInstDec decoded = chemu_dec_decode(inst, op.cls);

        // execute
        chiplog_debug("Executing '%s' (0x%04X)\n", op.name, inst);
        result = op.func(emu, &emu->memory, decoded);
    }

    return result;
}

void __updateTimer(ChipEmu emu, ChipReg *timerPtr) {
    mtx_lock(&emu->memlock);
    if (*timerPtr > 0)
        (*timerPtr)--;
    mtx_unlock(&emu->memlock);
}