#pragma once
// its assumend only one MetatileSet existis
#include <assert.h>
#include "tileset.h"
#include "coordinates.h"

typedef Tile Tile16x16[4];

typedef enum __attribute__((packed)) {
    META_EMPTY = 0,
    META_SKY = 1,
    META_BROKEN = 2,
    META_QUESTION = 3,
    META_BRICK = 4,
    META_BLOCK = 5,
    Metatile_Count
} Metatile;

static_assert(sizeof(Metatile) == 1, "Metatile enum must be packed");

typedef Tile16x16 MetatileSet[Metatile_Count];

extern const MetatileSet metatileset_main;

typedef struct {
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
} Metatile_Rect;

Tile metatile_tile_get(Metatile metatile, world_coord x, uint8_t y);
color metatile_pixel_copy(Metatile metatile, world_coord x, uint8_t y);

void metatile_full_copy(Metatile metatile, uint16_t* destination);
void metatile_ret_copy(Metatile metatile, uint16_t* destination, Metatile_Rect rect);
void metatile_ret_copy_vert(Metatile metatile, uint16_t* destination, uint8_t x1, uint8_t x2);


void metatile_line_copy(Metatile metatile, uint16_t** destination_ptr, uint8_t x1, uint8_t x2, uint8_t y);
void metatile_col_copy(Metatile metatile, uint16_t* destination, uint8_t x);
