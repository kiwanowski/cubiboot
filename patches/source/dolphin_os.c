#include "dolphin_os.h"

typedef void OSThread;

BOOL (*OSDisableInterrupts)() = (void*)0x8135bc70;
BOOL (*OSRestoreInterrupts)(BOOL) = (void*)0x8135bc98;
OSThread* (*SelectThread)(BOOL) = (void*)0x8135f5a4;

// from https://github.com/projectPiki/pikmin2/blob/de4b757bcb49e10b6d6822e6198be93c01890dcf/src/Dolphin/os/OSThread.c#L336-L343
void OSYieldThread() {
	BOOL enabled = OSDisableInterrupts();
	SelectThread(TRUE);
	OSRestoreInterrupts(enabled);
}
