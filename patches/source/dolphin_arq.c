#include "dolphin_arq.h"
#include "dolphin_os.h"

#include "reloc.h"
#include "attr.h"

// void aram_done_async(u32 request_address) {
//     OSReport("Copy Done %08x\n", request_address);
// }

// void aram_copy_async(ARQRequest *task, void *src, u32 dst, u32 size) {
//         void (*ARQPostRequest)(ARQRequest *task, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, ARQCallback callback) = (void*)0x8136ab80;
//         ARQPostRequest(task, 0, ARAM_DIR_MRAM_TO_ARAM, ARQ_PRIORITY_LOW, (u32)src, dst, size, &aram_done_async);
// }

// static OSCond aram_done_cond;
// void aram_done(u32 request_address) {
//     OSReport("Copy Done %08x\n", request_address);
//     OSSignalCond(&aram_done_cond);
// }

// __attribute_data__ ARQRequest aram_request;
// void aram_copy(void *src, u32 dst, u32 size) {
//     OSInitCond(&aram_done_cond);

//     void (*ARQPostRequest)(ARQRequest *task, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, ARQCallback callback) = (void*)0x8136ab80;
//     ARQPostRequest(&aram_request, 0, ARAM_DIR_MRAM_TO_ARAM, ARQ_PRIORITY_HIGH, (u32)src, dst, size, &aram_done);

//     OSWaitCond(&aram_done_cond, NULL);
// }
