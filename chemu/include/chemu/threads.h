// Simple implementation of C11's threads.h header
// Supporting:
//  * Threads (all thread functions and typedefs)
//  * Plain and Recursive mutexes (all functions except mtx_timedwait)
//  * Condition variables
// NOTE: This module is not part of the chemu API. Since most compilers
// do not support threads.h, this module serves as a stand-in replacement.

#ifndef _THREADS_H
#define _THREADS_H

#include <time.h>

#ifdef __unix__
#include <pthread.h>
#define _ST_UNIX
#elif defined(_WIN32)
#include <windows.h>
#define _ST_WIN
#endif


// NORETURN macro defines the function attribute noreturn
#if defined(_MSC_VER)
#define NORETURN __declspec(noreturn)  // Visual Studio compiler
#else
#define NORETURN _Noreturn             // C11 standard
#endif



// type definitions -----------------------------------------------------------

typedef int(*thrd_start_t)(void*);

typedef struct thrd_t_s {
    #ifdef _ST_UNIX
        pthread_t thread;
    #elif defined(_ST_WIN)
        HANDLE thread;
        DWORD threadId;
    #endif
} thrd_t;

enum {
    thrd_success,
    thrd_timedout,
    thrd_busy,
    thrd_nomem,
    thrd_error
};

#ifdef _ST_UNIX
    typedef pthread_mutex_t mtx_t;
#elif defined(_ST_WIN)
    typedef CRITICAL_SECTION mtx_t;
#endif

// 2 bit type specifier
// bit 0: 0 for plain, 1 for recursive
// bit 1: 1 for timed
enum {
    mtx_plain = 0,
    mtx_recursive = 1,
    mtx_timed = 2
};

#ifdef _ST_UNIX
    typedef pthread_cond_t cnd_t;
#elif defined(_ST_WIN)
    typedef CONDITION_VARIABLE cnd_t;
#endif

// thread functions -----------------------------------------------------------

int thrd_create(thrd_t*, thrd_start_t, void*);

int thrd_equal(thrd_t lhs, thrd_t rhs);

thrd_t thrd_current(void);

int thrd_sleep(const struct timespec* time_point, struct timespec* remaining);

void thrd_yield(void);

NORETURN void thrd_exit(int result);

int thrd_detach(thrd_t thr);

int thrd_join(thrd_t thr, int *res);

// mutex functions ------------------------------------------------------------

int mtx_init(mtx_t *mutex, int type);

int mtx_lock(mtx_t *mutex);

int mtx_trylock(mtx_t *mutex);

int mtx_unlock(mtx_t *mutex);

void mtx_destroy(mtx_t *mutex);

// condition variables --------------------------------------------------------

int cnd_init(cnd_t *cond);

int cnd_signal(cnd_t *cond);

int cnd_broadcast(cnd_t *cond);

int cnd_wait(cnd_t *cond, mtx_t *mutex);

int cnd_timedwait(cnd_t *cond, mtx_t *mutex, const struct timespec* time_point);

void cnd_destroy(cnd_t *cond);

#endif
