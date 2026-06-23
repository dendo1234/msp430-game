// its assumed only one Tileset exists
#pragma once
#include <assert.h>
#include <stdint.h>

static const uint8_t tile_width = 8;
static const uint8_t tile_height = 8;

typedef uint16_t color;

typedef const color Tile8x8[8*8];

typedef enum __attribute__((packed)) {
    TILE_EMPTY = 0,
    TILE_SKY = 1,
    TILE_BRICK = 2,

    TILE_BROKEN1,
    TILE_BROKEN2,
    TILE_BROKEN3,
    TILE_BROKEN4,

    TILE_BLOCK0,
    TILE_BLOCK1,
    TILE_BLOCK2,
    TILE_BLOCK3,

    TILE_QUESTION0,
    TILE_QUESTION1,
    TILE_QUESTION2,
    TILE_QUESTION3,

    TILE_COUNT,
} Tile;

static_assert(sizeof(Tile) == 1, "Tile enum must be packed");

typedef const Tile8x8 Tileset[TILE_COUNT];

extern const Tileset tileset_main;

void tile_ret_copy(Tile8x8 tile, uint16_t* destination, uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);
void tile_full_copy(Tile tile, uint16_t* destination);
