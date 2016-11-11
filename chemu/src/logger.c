#include "chemu/logger.h"
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>


typedef struct Logger_s {
    FILE *stream;
    ChemuLogLevel level;
    bool error;
} Logger;

static Logger logger = {
    .stream = NULL,
    .level = LOG_LEVEL_DEFAULT,
    .error = false
};

static const char * levelNames[] = {
    "DEBUG",
    "TRACE",
    "INFO",
    "WARNING",
    "ERROR"
};

void chemu_log_set(FILE *fp) {
    // set the stream if
    // fp is NULL (disables logging)
    // or
    // fp has not encountered an error
    if (fp == NULL || !ferror(fp)) {
        logger.stream = fp;
        logger.error = false;
    }
}

FILE* chemu_log_get() {
    return logger.stream;
}

ChemuLogLevel chemu_log_level() {
    return logger.level;
}

void chemu_log_setLevel(ChemuLogLevel level) {
    logger.level = level;
}

void chemu_log_write(ChemuLogLevel level, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    if (level == CHEMU_LOG_NONE)
        return;

    if (logger.stream != NULL && !logger.error && level >= logger.level) {
        time_t timer;
        time(&timer);
        struct tm *tm_info;

        #ifdef _WIN32
            struct tm tm_info_temp;
            localtime_s(&tm_info_temp, &timer);
            tm_info = &tm_info_temp;
        #else
            tm_info = localtime(&timer);
        #endif

        char timeStr[18];
        strftime(timeStr, 18, "%y-%m-%d %H:%M:%S", tm_info);

        fprintf(logger.stream, "[%s][%s]", timeStr, levelNames[(int)level]);
        vfprintf(logger.stream, fmt, ap);

        if (ferror(logger.stream))
            logger.error = true;
    }

    va_end(ap);
}
