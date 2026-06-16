#include <stdint.h>
#include "map/metatilemap.h"

typedef color Sprite8x8[64];

typedef struct {
    world_coord x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} Box;

typedef struct {
    Sprite8x8* data;
    Box box;
    bool flip;

} Sprite;

static const uint8_t sprite_slots_count = 8;

typedef struct {
    Sprite sprite_slots[sprite_slots_count];
    MetaMap* background;
    uint8_t used_slots;

} SpriteManager;

/*
index_x: (0:15)
index_y: (0:14)
*/
void sprite_render_dirty(uint16_t** destination, uint8_t index_x, uint8_t index_y);

bool sprite_check_inside(Box* box, world_coord x, uint8_t y);
bool sprite_check_inside_8x8(Box* box, uint8_t index_x, uint8_t index_y);
