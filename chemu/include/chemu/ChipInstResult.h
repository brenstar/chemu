#ifndef _CHIPINSTRESULT_H
#define _CHIPINSTRESULT_H

typedef enum ChipInstResult_e {
    INST_SUCCESS,           // successful execution
    INST_SUCCESS_INCR_PC,   // successful execution, increment program counter
    INST_FAILURE            // failed exectuion
} ChipInstResult;

#endif
