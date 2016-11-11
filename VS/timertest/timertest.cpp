// timertest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "chemu/timer.h"
#include <windows.h>
#include <stdio.h>

static BOOL CtrlHandler(DWORD fdwCtrlType);

static volatile BOOL interrupted = false;

int main() {

	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
		fprintf(stderr, "Failed to set control handler\n");
		return EXIT_FAILURE;
	}


	ChipTimer timer = chiptimer_create(255);
	chemu_timer_start(timer);
	int counter = 0;
	do {
		Sleep(2);

		if (++counter == 500) {
			int timerVal = chiptimer_get(timer);
			printf("Timer value is %d\n", timerVal);
			if (timerVal == 0) {
				printf("Resetting timer\n");
				chiptimer_set(timer, 255);
			}
			counter = 0;
		}


	} while (!interrupted);


	chemu_timer_stop(timer);
	chiptimer_destroy(timer);
    return EXIT_SUCCESS;
}


static BOOL CtrlHandler(DWORD fdwCtrlType) {
	(void)fdwCtrlType;
	interrupted = true;
	return FALSE;
}