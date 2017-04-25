
#include "p2dSettings.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

void p2dLog(const char* formatStr, ...) {
	va_list args;
	va_start(args, formatStr);
	vprintf(formatStr, args);
	va_end(args);
}
