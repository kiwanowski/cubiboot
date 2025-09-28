#include "dvd_threaded.h"
#include "dolphin_os.h"
#include "os.h"

// DI regs from YAGCD
#define DI_SR      0 // 0xCC006000 - DI Status Register
#define DI_SR_BRKINT     (1 << 6) // Break Complete Interrupt Status
#define DI_SR_BRKINTMASK (1 << 5) // Break Complete Interrupt Mask. 0:masked, 1:enabled
#define DI_SR_TCINT      (1 << 4) // Transfer Complete Interrupt Status
#define DI_SR_TCINTMASK  (1 << 3) // Transfer Complete Interrupt Mask. 0:masked, 1:enabled
#define DI_SR_DEINT      (1 << 2) // Device Error Interrupt Status
#define DI_SR_DEINTMASK  (1 << 1) // Device Error Interrupt Mask. 0:masked, 1:enabled
#define DI_SR_BRK        (1 << 0) // DI Break

#define DI_CVR     1 // 0xCC006004 - DI Cover Register (status2)
#define DI_CMDBUF0 2 // 0xCC006008 - DI Command Buffer 0
#define DI_CMDBUF1 3 // 0xCC00600c - DI Command Buffer 1 (offset in 32 bit words)
#define DI_CMDBUF2 4 // 0xCC006010 - DI Command Buffer 2 (source length)
#define DI_MAR     5 // 0xCC006014 - DMA Memory Address Register
#define DI_LENGTH  6 // 0xCC006018 - DI DMA Transfer Length Register
#define DI_CR      7 // 0xCC00601c - DI Control Register
#define DI_CR_RW     (1 << 2) // access mode, 0:read, 1:write
#define DI_CR_DMA    (1 << 1) // 0: immediate mode, 1: DMA mode (*1)
#define DI_CR_TSTART (1 << 0) // transfer start. write 1: start transfer, read 1: transfer pending (*2)

#define DI_IMMBUF  8 // 0xCC006020 - DI immediate data buffer (error code ?)
#define DI_CFG     9 // 0xCC006024 - DI Configuration Register

#define DVD_OEM_READ 0xA8000000

static vu32* const _di_regs = (vu32*)0xCC006000;

int dvd_threaded_read(void* dst, unsigned int len, uint64_t offset, unsigned int fd) {

    if (offset >> 2 > 0xFFFFFFFF) return -1;

    _di_regs[DI_SR] = (DI_SR_BRKINTMASK | DI_SR_TCINTMASK | DI_SR_DEINT | DI_SR_DEINTMASK);
	_di_regs[DI_CVR] = 0; // clear cover int

    _di_regs[DI_CMDBUF0] = DVD_OEM_READ | ((fd & 0xFF) << 16);
    _di_regs[DI_CMDBUF1] = offset >> 2;
	_di_regs[DI_CMDBUF2] = len;

	_di_regs[DI_MAR] = (u32)dst & 0x1FFFFFFF;
    _di_regs[DI_LENGTH] = len;
    _di_regs[DI_CR] = (DI_CR_DMA | DI_CR_TSTART); // start transfer

	while (_di_regs[DI_CR] & DI_CR_TSTART) {
        OSYieldThread();
    }

    DCInvalidateRange(dst, len);

    // check if ERR was asserted
	if (_di_regs[DI_SR] & DI_SR_DEINT) {
        return 1;
    }
	return 0;
}
