#include "metatilemap.h"
#include "metatileset.h"

#define min(x1, x2) (x1 < x2 ? x1 : x2)

Metatile metamap_tile_get(const MetaMap* meta_map, world_coord x, uint8_t y) {
    uint8_t tile_coord_y = y >> 4;
    uint8_t tile_coord_x = x >> 4; // uint8_t may be to small if Map is bigger than 256 tiles or 4096 pixels
    return meta_map->tiles[tile_coord_x + tile_coord_y * meta_map->width].tile;
}

const Metatile* metamap_tile_getref(const MetaMap* meta_map, world_coord x, uint8_t y) {
    uint8_t tile_coord_y = y >> 4;
    uint8_t tile_coord_x = x >> 4; // uint8_t may be to small if Map is bigger than 256 tiles or 4096 pixels
    return &(meta_map->tiles[tile_coord_x + tile_coord_y * meta_map->width].tile);
}

color metamap_pixel_copy(const MetaMap* meta_map, world_coord x, uint8_t y) {
    Metatile meta = metamap_tile_get(meta_map, x, y);
    metatile_pixel_copy(meta, x & 0xf, y);

}

// Metatile metamap_tile_get2(const MetamMap* meta_map, uint8_t tile, uint8_t y) {
//     return meta_map->tiles[tile + y * meta_map->width].tile;
// }

// void metamap_buffer_copy_vert16(const MetaMap* meta_map, uint16_t* buf, world_coord x1, world_coord x2) {
//     uint8_t tile_start = x1 & ~0xf;
//     uint8_t tile_end = x2 & ~0xf;
//     for (int j = 0; j < 16; j++) {
//         for (int i = x1; i <= x2; i++) {
//             Metatile metatile = metamap_tile_get(meta_map, i, j);
//             color pixel = metatile_pixel_copy(metatile, i * 0xf, j);
//             *buf++ = pixel;
//         }
//     }
// }

// void metamap_buffer_copy_vert16(const MetaMap* meta_map, uint16_t* buf, world_coord x1, world_coord x2) {
//     uint8_t tile_start = x1 & ~0xf;
//     uint8_t tile_end = x2 & ~0xf;
//     for (int j = 0; j < 16; j++) {


//         for (int tile = tile_start; tile <= x2; tile += 1 << 4) {
//             Metatile metatile = metamap_tile_get(meta_map, tile, j);

//             color pixel = metatile_pixel_copy(metatile, i * 0xf, j);

//             *buf++ = pixel;
//         }
//     }
// }

