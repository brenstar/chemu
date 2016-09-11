#include "chemu/logger.h"

#include <stdio.h>

static void testLog(const char * msg) {
	chiplog_debug("%s\n", msg);
	chiplog_trace("%s\n", msg);
	chiplog_info("%s\n", msg);
	chiplog_warn("%s\n", msg);
	chiplog_error("%s\n", msg);
}

int main() {

	const char *msg = "Test message";
	printf("testing log functions with message '%s'\n", msg);

	puts("Testing on stderr");
	chiplog_set(stderr);

	for (int i = 0; i < 5; ++i) {
		printf("Setting log level to %d\n", i);
		chiplog_setLevel((LogLevel)i);
		testLog(msg);
	}

	puts("testing on NULL");
	chiplog_set(NULL);
	chiplog_setLevel(CHIP_LOG_DEBUG);

	testLog(msg);



	return 0;
}