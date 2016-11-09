#ifndef _THREADUTIL_H
#define _THREADUTIL_H

#include "threads.h"

// thread utility module
// defines functions and types not present in threads.h

#ifdef __unix__
#define _TU_UNIX

#include <semaphore.h>


#elif defined _WIN32
#define _TU_WIN

#include <windows.h>

#define SEM_VALUE_MAX LONG_MAX

#endif


#ifdef _TU_UNIX
typedef sem_t ChipSemaphore;
typedef unsigned int SEM_VALUE_T;
#elif defined _WIN32
typedef HANDLE ChipSemaphore;
typedef LONG SEM_VALUE_T;
#endif

// ctu - chemu thread utils

//
// Initializes the given semaphore variable with the given count
// thrd_success is returned if the semaphore was successfully initialized,
// thrd_error otherwise.
//
int ctu_sem_init(ChipSemaphore *sem, SEM_VALUE_T initialCount);

//
// Destroys all resources used by the given semaphore.
// The behavior is undefined if there are threads waiting on the semaphore
// when this function is called.
//
void ctu_sem_destroy(ChipSemaphore *sem);

//
// Increases the semaphore count by 1 atomically for the given semaphore. The previouse
// count of the semaphore is stored in countVar if the pointer is not NULL
//
int ctu_sem_signal(ChipSemaphore *sem, SEM_VALUE_T *countVar);

//
// Suspends calling thread until the given semaphore's count is non-zero.
//
int ctu_sem_wait(ChipSemaphore *sem);

int ctu_sem_trywait(ChipSemaphore *sem);

#endif