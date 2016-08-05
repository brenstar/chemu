#ifndef _LOGGER_H
#define _LOGGER_H

#include "export.h"

#include <stdio.h>
#include <stdarg.h>

typedef enum LogLevel_e {
	CHIP_LOG_DEBUG = 0,
	CHIP_LOG_TRACE = 1,
	CHIP_LOG_INFO  = 2,
	CHIP_LOG_WARN  = 3,
	CHIP_LOG_ERROR = 4,
	CHIP_LOG_NONE  = 5
} LogLevel;

#define LOG_LEVEL_DEFAULT CHIP_LOG_INFO

CHEMU_API void chiplog_set(FILE *fp);

CHEMU_API FILE* chiplog_get();

CHEMU_API LogLevel chiplog_level();

CHEMU_API void chiplog_setLevel(LogLevel level);

CHEMU_API void chiplog_log(LogLevel level, const char *fmt, ...);

#ifdef NDEBUG
// time and level only
#define log(level, fmt, ...) chiplog_log(level, fmt, __VA_ARGS__)
#define LOG_PREFIX ""
#define LOG_PREFIX_ARGS
#else
// time, level and function name
#define log(level, fmt, ...) chiplog_log(level, "%s:%d:%s " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_PREFIX "%s:%d:%s "
#define LOG_PREFIX_ARGS __FILE__, __LINE__, __func__

#endif

#define chiplog_debug(fmt, ...) chiplog_log(CHIP_LOG_DEBUG, fmt, __VA_ARGS__)
#define chiplog_trace(fmt, ...) chiplog_log(CHIP_LOG_TRACE, fmt, __VA_ARGS__)
#define chiplog_info(fmt, ...) chiplog_log(CHIP_LOG_INFO, fmt, __VA_ARGS__)
#define chiplog_warn(fmt, ...) chiplog_log(CHIP_LOG_WARN, fmt, __VA_ARGS__)
#define chiplog_error(fmt, ...) chiplog_log(CHIP_LOG_ERROR, fmt, __VA_ARGS__)


#endif
