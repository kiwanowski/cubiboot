#include <gctypes.h>

// SCREEN RANGE
#define SCREEN_BOUND_TOP 224
#define SCREEN_BOUND_BOTTOM -224
#define SCREEN_BOUND_TOTAL_Y (SCREEN_BOUND_TOP - SCREEN_BOUND_BOTTOM)

#define SCREEN_BOUND_LEFT 320
#define SCREEN_BOUND_RIGHT -320
#define SCREEN_BOUND_TOTAL_X (SCREEN_BOUND_LEFT - SCREEN_BOUND_RIGHT)

#define DRAW_BOUND_TOP 104
#define DRAW_BOUND_BOTTOM 328

#define DRAW_OFFSET_Y 56
#define DRAW_TOTAL_ROWS 4

// ????
// #define GRID_LOAD_MAX_LINES (DRAW_TOTAL_ROWS + 2)

#define GRID_MOVE_SUCCESS 0
#define GRID_MOVE_FAIL 1

typedef struct {
    int pending_count;
    int direction;
    f32 remaining;
} anim_list_t;

typedef struct {
    anim_list_t anims;
    // int relative_index; // relative to top (needed??)
    f32 raw_position_y;
    f32 transparency;
    bool moving_out;
    bool moving_in;
} line_backing_t;

extern line_backing_t browser_lines[];

// helper
f32 get_position_after(line_backing_t *line_backing);

void grid_setup_func();
int grid_dispatch_navigate_up();
int grid_dispatch_navigate_down();
void grid_update_icon_positions();
