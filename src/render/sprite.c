#include "sprite.h"
#include "display.h"

SpriteManager manager;

void sprite_render_dirty(uint16_t** destination, uint8_t index_x, uint8_t index_y) {
    uint8_t index_world_x = index_x + (display_get_camera_pos() >> 3);

    Sprite* sprites_overlapping[sprite_slots_count];
    uint8_t sprites_overlapping_count = 0;
    for (int i = 0; i < sprite_slots_count; i++) {
        // check if any ovelaps the area
        Box* box = &manager.sprite_slots[i].box;
        if (sprite_check_inside_8x8(box, index_world_x, index_y)) {
            sprites_overlapping[sprites_overlapping_count] = &manager.sprite_slots[i];
            sprites_overlapping_count++;
        }
    }

    uint8_t y = index_y * 8;
    world_coord x = index_world_x << 3;

    if (sprites_overlapping_count == 0) {
        // do fast tilemap rendering
        Metatile meta = metamap_tile_get(manager.background, x, y);
        Tile tile = metatile_tile_get(meta, x, y);
        *destination = (uint16_t*)tileset_main[tile];
    }

    uint16_t* buf = *destination;

    // render
    for (int j = 0; j < 8; j++) {
        uint8_t current_y = y + j;
        for (int i = 0; i < 8; i++) {
            uint8_t current_x = x + i;
            for (int k = 0; k < sprites_overlapping_count; k++) {
                Sprite* sprite = sprites_overlapping[k];
                bool has = sprite_check_inside(&sprite->box, current_x, current_y);

                if (has) {
                    // get that pixel
                    uint8_t x_sprite = current_x - sprite->box.x;
                    uint8_t y_sprite = current_y - sprite->box.y;

                    color pixel = (*sprite->data)[x_sprite+y_sprite*8];
                    if (pixel) {
                        // not transparent
                        *buf++ = pixel;
                        break;
                    }
                }
            }
            // no pixel was found on the sprites, get the tilemap one
            metamap_pixel_copy(manager.background, current_x, current_y);
        }
    }
}

bool sprite_check_inside(Box* box, world_coord x, uint8_t y) {
    if (y >= box->y && y < box->y + box->h) {
        if (x >= box->x && x < box->x + box->w) {
            return true;
        }
    }
    return false;
}

bool sprite_check_inside_8x8(Box* box, uint8_t index_world_x, uint8_t index_y) {
    return (box->x >> 3 == index_world_x) && (box->y >> 3 == index_y);
}
