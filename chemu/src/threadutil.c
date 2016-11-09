
#include "chemu/threadutil.h"


int ctu_sem_init(ChipSemaphore* sem, SEM_VALUE_T initialCount) {
    #ifdef _TU_UNIX
        if (sem_init(sem, 0, initialCount))
            return thrd_error;
        else
            return thrd_success;
    #elif defined _TU_WIN
        HANDLE sHandle = CreateSemaphore(NULL, initialCount, SEM_VALUE_MAX, NULL);
        if (sHandle == NULL)
            return thrd_error;
        
        *sem = sHandle;
        return thrd_success;
    #endif
}

void ctu_sem_destroy(ChipSemaphore *sem) {
    #ifdef _TU_UNIX
        sem_destroy(sem);
    #elif defined _TU_WIN
        CloseHandle(*sem);
    #endif
}

int ctu_sem_signal(ChipSemaphore *sem, SEM_VALUE_T *countVar) {
    #ifdef _TU_UNIX
        int count;
        sem_getvalue(sem, &count);
        if (sem_post(sem) == -1)
            return thrd_error;
        else {
            if (countVar != NULL)
                *countVar = count;
            return thrd_success;
        }
    #elif defined _TU_WIN
        LONG count;
        if (ReleaseSemaphore(*sem, 1, &count)) {
            if (countVar != NULL)
                *countVar = count;
            return thrd_success;
        } else {
            return thrd_error;
        }
    #endif
}

int ctu_sem_wait(ChipSemaphore *sem) {
    #ifdef _TU_UNIX
        sem_wait(sem);
        return thrd_success;
    #elif defined _TU_WIN
        if (WaitForSingleObject(*sem, INFINITE) == WAIT_OBJECT_0)
            return thrd_success;
        else
            return thrd_error;
    #endif
}

int ctu_sem_trywait(ChipSemaphore *sem) {
    #ifdef _TU_UNIX
        if (sem_trywait(sem) == -1)
            return thrd_timedout;
        else
            return thrd_success;
    #elif defined _TU_WIN
        DWORD waitResult = WaitForSingleObject(*sem, 0);
        if (waitResult == WAIT_OBJECT_0)
            return thrd_success;
        else if (waitResult == WAIT_TIMEOUT)
            return thrd_timedout;
        else
            return thrd_error;
            
    #endif
}