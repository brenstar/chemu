#ifndef _CHEMULOGLEVEL_H
#define _CHEMULOGLEVEL_H

typedef enum ChemuLogLevel_e {
    CHEMU_LOG_DEBUG = 0,
    CHEMU_LOG_TRACE = 1,
    CHEMU_LOG_INFO  = 2,
    CHEMU_LOG_WARN  = 3,
    CHEMU_LOG_ERROR = 4,
    CHEMU_LOG_NONE  = 5
} ChemuLogLevel;

#endif