#include <stddef.h>
#include <stdarg.h>

#include "../attr.h"
#include "config.h"

#ifdef USE_NATIVE_SPRINTF
typedef void* (*WriteProc_t)(void*, const char*, size_t);

__attribute_reloc__ int (*__pformatter)(WriteProc_t WriteProc, void* WriteProcArg, const char* format_str, va_list arg);
__attribute_reloc__ WriteProc_t __StringWrite;

typedef struct {
	char* CharStr;
	size_t MaxCharCount;
	size_t CharsWritten;
} __OutStrCtrl;

// from https://github.com/projectPiki/pikmin2/blob/0285984b81a1c837063ae1852d94607fdb21d64c/src/Dolphin/MSL_C/MSL_Common/printf.c#L1267-L1310
int vsnprintf(char* s, size_t n, const char* format, va_list arg) {
	int end;
	__OutStrCtrl osc;
	osc.CharStr      = s;
	osc.MaxCharCount = n;
	osc.CharsWritten = 0;

	end = __pformatter(__StringWrite, &osc, format, arg);

	if (s) {
		s[(end < n) ? end : n - 1] = '\0';
	}

	return end;
}

int vsprintf(char* s, const char* format, va_list arg) {
	return vsnprintf(s, 0xFFFFFFFF, format, arg);
}

int snprintf(char* s, size_t n, const char* format, ...) {
	va_list args;
	va_start(args, format);
	return vsnprintf(s, n, format, args);
}

int sprintf(char* s, const char* format, ...) {
	va_list args;
	va_start(args, format);
	return vsnprintf(s, 0xFFFFFFFF, format, args);
}
#endif
