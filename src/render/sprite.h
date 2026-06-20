#pragma once
#include <stdint.h>
#include "map/metatilemap.h"

#define TRANSPOSE_8X8( \
    a0, a1, a2, a3, a4, a5, a6, a7, \
    b0, b1, b2, b3, b4, b5, b6, b7, \
    c0, c1, c2, c3, c4, c5, c6, c7, \
    d0, d1, d2, d3, d4, d5, d6, d7, \
    e0, e1, e2, e3, e4, e5, e6, e7, \
    f0, f1, f2, f3, f4, f5, f6, f7, \
    g0, g1, g2, g3, g4, g5, g6, g7, \
    h0, h1, h2, h3, h4, h5, h6, h7  \
) { \
    a0, b0, c0, d0, e0, f0, g0, h0, \
    a1, b1, c1, d1, e1, f1, g1, h1, \
    a2, b2, c2, d2, e2, f2, g2, h2, \
    a3, b3, c3, d3, e3, f3, g3, h3, \
    a4, b4, c4, d4, e4, f4, g4, h4, \
    a5, b5, c5, d5, e5, f5, g5, h5, \
    a6, b6, c6, d6, e6, f6, g6, h6, \
    a7, b7, c7, d7, e7, f7, g7, h7  \
}

typedef color Sprite8x8[64];

typedef struct {
    world_coord x1;
    world_coord x2;
    uint8_t y1;
    uint8_t y2;
} Bounds;

typedef struct {
    world_coord x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} Rect;

typedef struct {
    world_coord x;
    uint8_t y;
} Offset;

typedef struct {
    const Sprite8x8* data;
    Offset offset;
    bool flip;
    bool render;
} Sprite;

typedef struct {
    Rect box;
    bool flip;
    bool render;
    uint8_t sprite_count;
    Sprite sprites[];
} MetaSprite;


static const uint8_t sprite_slots_count = 8;

typedef struct {
    MetaSprite* sprite_slots[sprite_slots_count];
    const MetaMap* background;
    uint8_t used_slots;

} SpriteManager;

extern SpriteManager sprite_manager;

/*
index_x: (0:15)
index_y: (0:14)
*/

void sprite_render_dirty8x8(uint16_t* destination, world_coord tile_x1, uint8_t y);

void sprite_set_dirty(uint8_t dirty[4][30], Bounds* box);
void metasprite_set_dirty(uint8_t dirty[4][30], MetaSprite* meta);
