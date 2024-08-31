#include "dolphin_arq.h"
#include "dolphin_os.h"

#include "reloc.h"
#include "attr.h"

__attribute_reloc__ void (*ARQPostRequest)(ARQRequest *task, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, ARQCallback callback);

void dolphin_ARQPostRequest(ARQRequest *task, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, ARQCallback callback) {
    ARQPostRequest(task, owner, type, priority, source, dest, length, callback);
}
