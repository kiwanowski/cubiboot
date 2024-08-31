#include <gctypes.h>

#include "dolphin_arq.h"
#include "icon.h"
#include "bnr.h"

// Backing
typedef enum {
    GM_LOAD_STATE_NONE,
    GM_LOAD_STATE_SETUP, // valid for use
    GM_LOAD_STATE_UNLOADED,
    GM_LOAD_STATE_UNLOADING,
    GM_LOAD_STATE_LOADING,
    GM_LOAD_STATE_LOADED, // valid for use
} gm_load_state_t;

typedef enum {
    GM_FILE_TYPE_UNKNOWN,
    GM_FILE_TYPE_DIRECTORY,
    GM_FILE_TYPE_PROGRAM,
    GM_FILE_TYPE_GAME
} gm_file_type_t;

typedef struct {
    u32 used;
    u8 data[ICON_PIXELDATA_LEN];
} gm_icon_buf_t;

typedef struct {
    u32 used;
    u8 data[BNR_PIXELDATA_LEN];
} gm_banner_buf_t;

typedef struct {
    ARQRequest req;
    u32 aram_offset;
    gm_load_state_t state;
    gm_icon_buf_t *buf;
} gm_icon_t;

typedef struct {
    ARQRequest req;
    u32 aram_offset;
    gm_load_state_t state;
    gm_banner_buf_t *buf;
} gm_banner_t;

typedef struct {
    gm_icon_t icon;
    gm_banner_t banner;
} gm_asset_t;

typedef struct {
    u8 game_id[6];
    u8 dvd_bnr_type;
	u32 dvd_bnr_offset;
} gm_extra_t;

typedef struct {
    char path[128];
    gm_file_type_t type;
} gm_path_entry_t;

typedef struct {
    char path[128];
    BNRDesc desc;
    gm_extra_t extra;
    gm_asset_t asset;
    gm_file_type_t type;
} gm_file_entry_t;

void gm_start_thread();
