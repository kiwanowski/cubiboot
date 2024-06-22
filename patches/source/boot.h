#include <gctypes.h>

typedef struct {
    u32 max_addr;
    void *entrypoint;
} dol_info_t;

dol_info_t load_dol(char *path, bool flash);

void *load_apploader();
// uint8_t prepare_game(char *boot_path);
// void chainload_boot_game(uint8_t fd);
void prepare_game_lowmem(char *boot_path);
void chainload_boot_game(char *boot_path);

void run(register void* entry_point);
