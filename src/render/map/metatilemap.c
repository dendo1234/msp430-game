#include "metatilemap.h"
#include "metatileset.h"

#define min(x1, x2) (x1 < x2 ? x1 : x2)

Metatile metamap_metatile_get(const MetaMap* meta_map, world_coord x, uint8_t y) {
    uint8_t tile_coord_y = y >> 4;
    uint16_t tile_coord_x = x >> 4;
    return meta_map->tiles[tile_coord_x + tile_coord_y * meta_map->width].tile;
}

Metatile metamap_metatile_get2(const MetaMap* meta_map, world_coord tile_x, uint8_t tile_y) {
    return meta_map->tiles[tile_x + tile_y * meta_map->width].tile;
}

Tile metamap_tile_get(const MetaMap* meta_map, world_coord x, uint8_t y) {
    uint16_t tile_x = x & 0x7;
    uint8_t tile_y = y & 0x7;
    uint8_t metatile_x = x >> 4;
    uint16_t metatile_y = y >> 4;
    Metatile meta = meta_map->tiles[metatile_x + metatile_y * meta_map->width].tile;
    return metatileset_main[meta][tile_x + tile_y*2];
}

const Metatile* metamap_metatile_getref(const MetaMap* meta_map, world_coord x, uint8_t y) {
    uint8_t tile_coord_y = y >> 4;
    uint8_t tile_coord_x = x >> 4; // uint8_t may be to small if Map is bigger than 256 tiles or 4096 pixels
    return &(meta_map->tiles[tile_coord_x + tile_coord_y * meta_map->width].tile);
}
