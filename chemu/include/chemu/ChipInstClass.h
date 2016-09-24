#ifndef _CHIPINSTCLASS_H
#define _CHIPINSTCLASS_H

// Enumeration of instruction classes used to simplify decoding.
typedef enum ChipInstClass_e {
    INST_CLASS_A,       // address class
    INST_CLASS_I,       // immediate class
    INST_CLASS_R,       // register class
    INST_CLASS_V        // void class
} ChipInstClass;

#endif
