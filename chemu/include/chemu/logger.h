#ifndef _LOGGER_H
#define _LOGGER_H

#include "internal/export.h"
#include "ChemuLogLevel.h"

#include <stdio.h>
#include <stdarg.h>

//typedef enum LogLevel_e {
//    CHIP_LOG_DEBUG = 0,
//    CHIP_LOG_TRACE = 1,
//    CHIP_LOG_INFO  = 2,
//    CHIP_LOG_WARN  = 3,
//    CHIP_LOG_ERROR = 4,
//    CHIP_LOG_NONE  = 5
//} LogLevel;

#define LOG_LEVEL_DEFAULT CHEMU_LOG_INFO

CHEMU_API void chemu_log_set(FILE *fp);

CHEMU_API FILE* chemu_log_get();

CHEMU_API ChemuLogLevel chemu_log_level();

CHEMU_API void chemu_log_setLevel(ChemuLogLevel level);

CHEMU_API void chemu_log_write(ChemuLogLevel level, const char *fmt, ...);

#ifdef NDEBUG
// time and level only
#define log(level, fmt, ...) chemu_log_write(level, fmt, __VA_ARGS__)
#define LOG_PREFIX ""
#define LOG_PREFIX_ARGS
#else
// time, level and function name
#define log(level, fmt, ...) chemu_log_write(level, "%s:%d:%s " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_PREFIX "%s:%d:%s "
#define LOG_PREFIX_ARGS __FILE__, __LINE__, __func__

#endif

#define chiplog_debug(fmt, ...) chemu_log_write(CHEMU_LOG_DEBUG, fmt, ##__VA_ARGS__)
#define chiplog_trace(fmt, ...) chemu_log_write(CHEMU_LOG_TRACE, fmt, ##__VA_ARGS__)
#define chiplog_info(fmt, ...) chemu_log_write(CHEMU_LOG_INFO, fmt, ##__VA_ARGS__)
#define chiplog_warn(fmt, ...) chemu_log_write(CHEMU_LOG_WARN, fmt, ##__VA_ARGS__)
#define chiplog_error(fmt, ...) chemu_log_write(CHEMU_LOG_ERROR, fmt, ##__VA_ARGS__)

#endif
