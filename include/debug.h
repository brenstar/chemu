/* Version: $Id: debug.h,v 1.4 2014/11/08 06:47:05 bjr1251 Exp $
 *
 * debug.h
 * A collection of macros for the printing of debugging messages to stderr as
 * well as logging macros. To disable debugging, define NDEBUG. To disable
 * logging, define NLOG. The default log level is INFO, this can be changed by
 * redefining LOGLEVEL.
 *
 * Author: Brennan Ringey : bjr1251
 */

#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

#define LOG_LEVEL_ERROR    3
#define LOG_LEVEL_WARNING  2
#define LOG_LEVEL_INFO     1
#define LOG_LEVEL_TRACE    0

// Debug macro

#ifndef NDEBUG
#define debug(fmt, ...) fprintf(stderr, "[DEBUG] %s:%d:%s(): " fmt, \
                                __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif // DEBUG

// Logging macros

#ifndef LOGLEVEL
#define LOGLEVEL LOG_LEVEL_INFO
#endif //LOGLEVEL

#ifdef NLOG
#define log(lev, ln, fmt, ...)
#else
#define log(lev, ln, fmt, ...) if (lev >= LOGLEVEL) \
    fprintf(stderr, "[%s] " fmt, ln, ##__VA_ARGS__)
#endif // NLOG

#define logTrace(fmt, ...) log(LOG_LEVEL_TRACE, "TRACE", fmt, ##__VA_ARGS__)
#define logInfo(fmt, ...) log(LOG_LEVEL_INFO, "INFO", fmt, ##__VA_ARGS__)
#define logWarn(fmt, ...) log(LOG_LEVEL_WARNING, "WARNING", fmt, ##__VA_ARGS__)
#define logError(fmt, ...) log(LOG_LEVEL_ERROR, "ERROR", fmt, ##__VA_ARGS__)


#endif // _DEBUG_H
