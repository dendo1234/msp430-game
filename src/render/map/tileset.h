// its assumed only one Tileset exists
#pragma once
#include "render/display.h"
#include <assert.h>

static const uint8_t tile_width = 8;
static const uint8_t tile_height = 8;

typedef const color Tile8x8[8*8];

typedef enum __attribute__((packed)) {
    TILE_EMPTY = 0,
    TILE_SKY = 1,
    TILE_BRICK = 2,
    TILE_COUNT,
} Tile;

static_assert(sizeof(Tile) == 1, "Tile enum must be packed");

typedef const Tile8x8 Tileset[TILE_COUNT];

extern const Tileset tileset_main;

void tile_ret_copy(Tile8x8 tile, uint16_t* destination, uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);
void tile_full_copy(Tile tile, uint16_t* destination);
