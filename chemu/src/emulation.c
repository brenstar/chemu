#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "chemu/emulation.h"
#include "chemu/decode.h"
#include "chemu/display.h"
#include "chemu/memory.h"
#include "chemu/stack.h"
#include "chemu/timer.h"
#include "chemu/logger.h"


#if defined(__unix__)
#include <unistd.h>
#elif defined(_WIN32)
#endif

//static void mainLoop_noLimit(ChipEmu *emu, int *exitStatus);
//static void mainLoop_limit(ChipEmu *emu, int *exitStatus);

void chipemu_init(ChipEmu *emu) {
    // set callbacks to NULL
    emu->pollKeyHandler = NULL;
    emu->redrawCallback = NULL;

    emu->running = false;

    emu->speed = CHIPEMU_DEFAULT_SPEED;

    // set timers to NULL
    emu->soundTimer = NULL;
    emu->delayTimer = NULL;

	chipmem_init(&emu->memory);
    chipemu_reset(emu);
}

ChipKey chipemu_getKey(ChipEmu *emu) {
    if (emu->pollKeyHandler != NULL)
        return emu->pollKeyHandler(emu);
    return CHIP_KEY_0;
}

int chipemu_loadROM(ChipEmu *emu, const char *path) {
    int bytesRead = -1;
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
            return -1; // ROM file was too big
        memcpy(emu->memory.array + CHIP_PRGM_START, buffer, bytesRead);
        fclose(fp);
    }
    return bytesRead;
}

int chipemu_mainLoop(ChipEmu *emu) {

    int exitStatus = EXIT_SUCCESS;
    //emu->running = true;

    //emu->soundTimer = chiptimer_create(0);
    //emu->delayTimer = chiptimer_create(0);

    //chiptimer_start(emu->soundTimer);
    //chiptimer_start(emu->delayTimer);

    //if (emu->speed == 0)
    //    mainLoop_noLimit(emu, &exitStatus);
    //else
    //    mainLoop_limit(emu, &exitStatus);

    //// timer_destroy calls timer_stop automatically
    //chiptimer_destroy(emu->soundTimer);
    //chiptimer_destroy(emu->delayTimer);

    //emu->soundTimer = NULL;
    //emu->delayTimer = NULL;

    return exitStatus;
}

int chipemu_step(ChipEmu *emu) {
    int result = CHIP_STEP_SUCCESS;

    // fetch
    ChipAddress pc = emu->memory.reserved.pc;
    ChipInst instruction = (uint16_t)(emu->memory.array[pc] << 8 |
                                      emu->memory.array[pc + 1]);


    // decode
    int i = chipdec_index(instruction);
	if (i == NO_INSTRUCTION) {
		chiplog_error("Illegal instruction: %04X\n", instruction);
		return CHIP_STEP_FAILURE;
	}

    ChipOp op = CHIP_OPTABLE[i];
    ChipInstDec decoded = chipdec_decode(instruction, op.cls);

    // execute
	chiplog_debug("Executing '%s' (0x%04X) at 0x%03X\n", op.name, instruction, pc);
    ChipInstResult instResult = op.func(emu, decoded);
    switch (instResult) {
        case INST_SUCCESS:
            break;
        case INST_SUCCESS_INCR_PC:
            emu->memory.reserved.pc += 2;
            break;
        case INST_FAILURE:
            chiplog_error("Instruction index %d has failed.\n", i);
            result = CHIP_STEP_FAILURE;
            break;
    }

    return result;
}


void chipemu_redraw(ChipEmu *emu) {
    if (emu->redrawCallback != NULL)
        emu->redrawCallback(emu);
}

void chipemu_reset(ChipEmu *emu) {
    if (emu->running)
        return;

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
}


//#define mainloop(code)                                \
//    do {                                              \
//        if (chipemu_step(emu) == CHIP_STEP_FAILURE) { \
//            *exitStatus = EXIT_FAILURE;               \
//            break;                                    \
//        }                                             \
//        code                                          \
//    } while (emu->running)
//
//
//static void mainLoop_limit(ChipEmu *emu, int *exitStatus) {
//    const int interval = 1000000 / emu->speed;
//    clock_t currentTime, lastTime;
//    lastTime = clock();
//
//    mainloop(
//        currentTime = clock();
//        int sleepTime = interval - ((currentTime - lastTime) * 1000000 / CLOCKS_PER_SEC);
//        lastTime = currentTime;
//		#ifdef __unix__
//        if (sleepTime > 0)
//            usleep(sleepTime);
//		#endif
//    );
//}
//
//static void mainLoop_noLimit(ChipEmu *emu, int *exitStatus) {
//    mainloop();
//}
