#include "tilemap.h"
#include <assert.h>

Tile tilemap_tile_get(const Tilemap* tilemap, world_coord x, uint8_t y) {
    uint8_t tile_coord_y = y >> 3;
    uint8_t tile_coord_x = x >> 3; // uint8_t may be to small if Map is big
    return tilemap->map->tiles[tile_coord_x + tile_coord_y * map_width];
}

uint16_t tilemap_buffer_required_size(world_coord x1, uint8_t y1, world_coord x2, uint8_t y2) {
    assert(x2 >= y1);
    assert(y2 >= y1);
    return (x2 - x1 + 1) * (y2 - y1 + 1);
}

void tilemap_buffer_copy(const Tilemap *tilemap, uint16_t* buf, world_coord x1, uint8_t y1, world_coord x2, uint8_t y2) {
    for (uint8_t j = y1; j <= y2; j++) {
        for (world_coord i = x1; i <= x2; i++) {
            Tile current_tile = tilemap_tile_get(tilemap, x1, y1); 
            *buf++ = (*tilemap->tileset)[current_tile][(i & 0x7) + (j & 0x7)*8];
        }
    }
}

color tilemap_color_picker(world_coord x, uint8_t y) {
    uint16_t buf;
    tilemap_buffer_copy(&tilemap1, &buf, x, y, x, y);
    return buf;
}

// void tilemap_copy_to_buffer(Tilemap* tilemap, world_coord src_x, uint8_t src_y, uint8_t width, uint8_t height, uint8_t* buf) {
//     for (int j = 0; j < height; j++) {
//         for (int i = 0; i < width; i++) {
//             tilemap.tileset
//             *buf = ;

//         }
//     }

// }
