#include <stdint.h>
#include "map/metatilemap.h"

typedef color Sprite8x8[64];

typedef struct {
    world_coord x1;
    world_coord x2;
    uint8_t y1;
    uint8_t y2;
} Box;

typedef struct {
    Sprite8x8* data;
    Box box;
    bool flip;
    bool render;
} Sprite;

// typedef struct {
//     Box box;
//     uint8_t width;
//     bool flip;
//     uint8_t sprite_count;
//     Sprite sprites[]
// } MetaSprite;


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

void sprite_render_column(uint16_t* destination, world_coord x, uint8_t y);

void sprite_ret_copy(uint16_t* destination, Sprite* sprite, Box* src_box);
