#include <string.h>

#include "picolibc.h"
#include "time.h"

#include "reloc.h"
#include "paths.h"
#include "config.h"

#include "gc_dvd.h"

#define SD_TARGET_PATH "/"

// default
// favorites
// alphabetical (optionally show letter?)

typedef struct {
    bool is_gcm;
    bool is_valid;
    u32 fst_offset;
    u32 fst_size;
    u32 mem_bnr_offset;
    u32 aram_bnr_offset;
    char iso_name[64];
    char iso_path[128];
} game_backing_entry_t;

int cmp_iso_path(const void* ptr_a, const void* ptr_b){
    const game_backing_entry_t *obj_a = ptr_a;
    const game_backing_entry_t *obj_b = ptr_b;
    return strcmp(obj_a->iso_path, obj_b->iso_path);
}

game_backing_entry_t raw_game_backing_list[1000];
game_backing_entry_t *game_backing_list[1000];

void weird_panic() {
    OSReport("PANIC: unknown\n");
    while(1);
}

void file_enum_worker() {
    u64 start_time = gettime();
    // dvd_custom_open(IPC_DEVICE_SD, SD_TARGET_PATH, FILE_ENTRY_TYPE_DIR, 0); // reset readdir

    // // TODO: fallback to WiFi
    // file_status_t status;
    // dvd_custom_status(IPC_DEVICE_SD, &status);
    // if (status.result != 0) {
    //     OSReport("PANIC: SD Card could not be opened\n");
    //     while(1);
    // }

    OSReport("File enum:\n");

    file_entry_t ent;
    int current_ent_index = 0;
    int ret = 0;
    while(1) {
        ret = dvd_custom_readdir(IPC_DEVICE_SD, &ent);
        if (ret != 0) weird_panic();
        if (ent.name[0] == 0)
            break;
        if (ent.type != 0)
            continue;
        if (strcmp(ent.name, "boot.iso") == 0)
            continue;

        char *ext = FileSuffix(ent.name);
        if (strcmp(ext, ".iso") != 0)
            continue;

#ifdef PRINT_READDIR_NAMES
        OSReport("READDIR ent(%u): %s [len=%d]\n", ent.type, ent.name, strlen(ent.name));
#endif

        game_backing_entry_t *game_backing = &raw_game_backing_list[current_ent_index];
        strcpy(game_backing->iso_path, SD_TARGET_PATH);
        strcat(game_backing->iso_path, ent.name);
        game_backing->iso_name[0] = '\0'; // zero name string

        // ret = dvd_custom_open(IPC_DEVICE_SD, iso_path, FILE_ENTRY_TYPE_FILE, IPC_FILE_FLAG_DISABLECACHE | IPC_FILE_FLAG_DISABLEFASTSEEK);
        // OSReport("OPEN ret: %08x\n", ret);

        game_backing_list[current_ent_index] = game_backing;
        current_ent_index++;
    }

    f32 runtime = (f32)diff_usec(start_time, gettime()) / 1000.0;
    OSReport("File enum completed! took=%f (%d)\n", runtime, current_ent_index);

    start_time = gettime();
    qsort(game_backing_list, current_ent_index, sizeof(game_backing_entry_t*), &cmp_iso_path);

    runtime = (f32)diff_usec(start_time, gettime()) / 1000.0;
    OSReport("Sort took=%f\n", runtime);

    OSReport("[DONE] - KILL DOLPHIN NOW\n");
    return;
}
