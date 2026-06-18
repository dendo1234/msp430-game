#include <assert.h>
#include "metatileset.h"
#include "coordinates.h"

const MetatileSet metatileset_main = {
    [META_SKY] = {
        TILE_SKY, TILE_SKY,
        TILE_SKY, TILE_SKY
    },
    [META_BRICK] = {
        TILE_BRICK, TILE_BRICK,
        TILE_BRICK, TILE_BRICK
    },
};

Tile metatile_tile_get(Metatile metatile, world_coord x, uint8_t y) {
    return metatileset_main[metatile][((x >> 2) & 0x2) + (y >> 2 & 0x2)];
}

void metatile_full_copy(Metatile metatile, uint16_t* destination) {
    const Tile* tiles = metatileset_main[metatile];

    for (int j = 0; j < 2; j++) {
        const color* q1 = tileset_main[*tiles++];
        const color* q2 = tileset_main[*tiles++];
        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 8; k++) {
                *destination++ = *q1++;
            }
            for (int k = 0; k < 8; k++) {
                *destination++ = *q2++;
            }
        }
    }
}

void metatile_ret_copy(Metatile metatile, uint16_t* destination, Metatile_Rect rect) {
    const Tile* tiles = metatileset_main[metatile];

    uint8_t x1 = rect.x1;
    uint8_t x2 = rect.x2;
    uint8_t y1 = rect.y1;
    uint8_t y2 = rect.y2;
    assert(x1 <= x2 && x2 < 16 && y1 <= y2 && y2 < 16);
    
    uint8_t div = x2 > 7 ? 7 : x2;
    uint8_t div2 = x1 > 8 ? x1 : 8;
    // uint8_t div2 = y2 > 7 ? 7 : y2;
    
    #pragma MUST_ITERATE(1,2,1)
    for (int j = y1 >> 3; j <= y2 >> 3; j++) {
        const color* q1 = tileset_main[*(tiles + j)];
        const color* q2 = tileset_main[*(tiles + j)];
        #pragma MUST_ITERATE(1, 8, 1)
        for (int i = 0; i < 8; i++) {
            #pragma MUST_ITERATE(0,8,1)
            for (int k = x1; k <= div; k++) {
                *destination++ = q1[k + i*8];
            }
            #pragma MUST_ITERATE(0,8,1)
            for (int k = div2; k <= x2; k++) {
                *destination++ = q2[k-8 + i*8];
            }
        }
    }
}

void metatile_ret_copy_vert(Metatile metatile, uint16_t* destination, uint8_t x1, uint8_t x2) {
    const Tile* tiles = metatileset_main[metatile];

    assert(x1 <= x2 && x2 < 16);
    
    uint8_t left_count = 0;
    uint8_t right_count = 0;

    if (x1 < 8) {
        left_count = x2 > 7 ? 7 - x1 : x2 - x1;
        left_count++;
    }

    if (x2 > 7) {
        right_count = x1 > 7 ? x2 - x1 : x2 - 8;
        right_count++;
    }

    uint8_t left_offset = 8 - left_count;
    uint8_t right_offset = 8 - right_count;

    for (int j = 0; j <= 1; j++) {
        const color* q1 = tileset_main[tiles[j*2]] + x1;
        const color* q2 = tileset_main[tiles[j*2 + 1]] + (x2 - right_count - 7);
        for (int i = 0; i < 8; i++) {
            uint8_t count = left_count;
            while (count) {
                *destination++ = *q1++;
                count--;
            }
            count = right_count;
            while (count) {
                *destination++ = *q2++;
                count--;
            }
            q1 += left_offset;
            q2 += right_offset;
        }
    }
}

void metatile_col_copy(Metatile metatile, uint16_t* destination, uint8_t x) {
    const Tile* tiles = metatileset_main[metatile];
    const Tile* tile;
    if (x < 8) {
        tile = tiles;
    } else {
        tile = tiles + 1;
        x -= 8;
    }

    for (int j = 0; j <= 1; j++) {
        const color* q = tileset_main[tile[j*2]] + x*8;
        for (int i = 0; i < 8; i++) {
            *destination++ = *q++;
        }
    }
}

color metatile_pixel_copy(Metatile metatile, world_coord x, uint8_t y) {
    Tile tile = metatileset_main[metatile][((x >> 2) & 0x2) + ((y >> 2) & 0x2)];
    return tileset_main[tile][(x & 0x7) + (y & 0x7)*8];
}

void metatile_line_copy(Metatile metatile, uint16_t** destination_ptr, uint8_t x1, uint8_t x2, uint8_t y) {
    assert(x1 <= x2 && x2 < 16);
    assert(y < 16);
    uint8_t offset = (y & 0x7) * 8;
    uint16_t* buf = *destination_ptr;

    const Tile* tiles = metatileset_main[metatile];

    Tile x1_tile = tiles[(x1 >> 3) + (y >> 3)];
    Tile x2_tile = tiles[(x2 >> 3) + (y >> 3)];

    if (x1_tile != x2_tile) {
        const color* source = &tileset_main[x1_tile][offset + x1];
        while (x1 <= 0x7) {
            *buf++ = *source++;
            x1++;
        }
        x1 = 0;
    }

    const color* source = &tileset_main[x2_tile][offset + x1];
    while (x1 <= x2) {
        *buf++ = *source++;
        x1++;
    }
}
