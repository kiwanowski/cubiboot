#ifndef TWEAKS_H
#define TWEAKS_H

#include <stdint.h>
#include <stdbool.h>
#ifdef IPL_CODE
#include "../games.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

const char* emu_get_device();

#ifdef IPL_CODE
void emu_update_boot();
bool emu_can_boot(gm_file_type_t type);
void emu_draw_boot_error(gm_file_type_t type, u8 ui_alpha);
bool emu_has_dvd();
#else
void ensure_ipl_loaded(uint8_t* bios_buffer);
#endif

#ifdef __cplusplus
}
#endif

#endif // TWEAKS_H
