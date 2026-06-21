#include "sprite.h"
#include "display.h"

SpriteManager sprite_manager;

bool sprite_check_inside(Rect* check, world_coord tile_x1, world_coord tile_x2) {
    world_coord x1 = check->x;
    world_coord x2 = x1 + check->w - 1;

    return (x1 >= tile_x1 && x1 <= tile_x2) || (x2 >= tile_x1 && x2 <= tile_x2) || (x1 <= tile_x1 && x2 >= tile_x2);
}

void sprite_ret_copy(uint16_t* destination, Sprite* sprite, Bounds* src_box, Bounds* dst_box) {
    const color* data = (*sprite->data);

    destination += dst_box->y1;
    destination += dst_box->x1*8;

    data += src_box->y1;
    data += src_box->x1*8;

    uint8_t count_x = src_box->x2 - src_box->x1 + 1;
    uint8_t count_y = src_box->y2 - src_box->y1 + 1;

    uint8_t stride = 8 - count_y;

    while (count_x)
    {
        int i = count_y;
        while(i) {
            if (*data) {
                *destination = *data;
            }
            destination++;
            data++;
            i--;
        }
        destination += stride;
        data += stride;
        count_x--;
    }
};

void sprite_render_dirty8x8(uint16_t* destination, world_coord tile_x1, uint8_t y) {
    world_coord tile_x2 = tile_x1 + 7;
    uint8_t tile_y1 = y & ~0x7;
    uint8_t tile_y2 = y + 7;

    Tile tile = metamap_tile_get(sprite_manager.background, tile_x1, tile_y1);

    tile_full_copy(tile, destination);

    // render

    for (int i = 0; i < sprite_slots_count; i++) {
        MetaSprite* meta_sprite = sprite_manager.sprite_slots[i];

        if (!meta_sprite->render) {
            continue;
        }

        Rect* rect = &meta_sprite->box;
        bool render_meta = sprite_check_inside(rect, tile_x1, tile_x2);

        if (render_meta) {
            uint8_t sprite_count = meta_sprite->sprite_count;

            for (int i = 0; i < sprite_count; i++) {
                Sprite* sprite = &meta_sprite->sprites[i];
                if (!sprite->render) {
                    continue;
                }
                
                world_coord x1 = rect->x + sprite->offset.x;
                uint8_t y1 = rect->y + sprite->offset.y;
                world_coord x2 = x1 + 7;
                uint8_t y2 = y1 + 7;

                Bounds dst_box;
                Bounds src_box;

                if (x1 >= tile_x1 && x1 <= tile_x2) {
                    dst_box.x1 = x1 & 0x7;
                    dst_box.x2 = 7;

                    src_box.x1 = 0;
                    src_box.x2 = 7 - (x1 & 0x7);
                } else if (x2 >= tile_x1 && x2 <= tile_x2) {
                    dst_box.x1 = 0;
                    dst_box.x2 = x2 & 0x7;

                    src_box.x1 = 7 - (x2 & 0x7);
                    src_box.x2 = 7;
                } else {
                    continue;
                }

                if (y1 >= tile_y1 && y1 <= tile_y2) {
                    dst_box.y1 = y1 & 0x7;
                    dst_box.y2 = 7;

                    src_box.y1 = 0;
                    src_box.y2 = 7 - (y1 & 0x7);
                } else if (y2 >= tile_y1 && y2 <= tile_y2) {
                    dst_box.y1 = 0;
                    dst_box.y2 = y2 & 0x7;

                    src_box.y1 = 7 - (y2 & 0x7);
                    src_box.y2 = 7;
                } else {
                    continue;
                }

                sprite_ret_copy(destination, sprite, &src_box, &dst_box);

            }
        }
    }
}


void sprite_set_dirty(uint8_t dirty[4][30], Bounds* box) {
    world_coord camera_pos = display_get_camera_pos();
    world_coord camera_tile_offset = camera_pos >> 3;
    uint8_t y1 = box->y1 >> 3;
    uint8_t y2 = box->y2 >> 3;
    if (coord_cull(box->x1, camera_pos)) {
        uint8_t tile_x = (box->x1 >> 3) - camera_tile_offset;

        dirty[tile_x >> 3][y1] |= 1 << (tile_x & 0x7);
        dirty[tile_x >> 3][y2] |= 1 << (tile_x & 0x7);
    }

    if (coord_cull(box->x2, camera_pos)) {
        uint8_t tile_x = (box->x2 >> 3) - camera_tile_offset;

        dirty[tile_x >> 3][y1] |= 1 << (tile_x & 0x7);
        dirty[tile_x >> 3][y2] |= 1 << (tile_x & 0x7);
    }
}

void metasprite_set_dirty(uint8_t dirty[4][30], MetaSprite* meta) {
    world_coord camera_pos = display_get_camera_pos();
    uint8_t sprite_count = meta->sprite_count;

    world_coord meta_x1 = meta->box.x;
    uint8_t meta_y1 = meta->box.y;
    for (int i = 0; i < sprite_count; i++) {
        Sprite* sprite = &meta->sprites[i];

        world_coord x1 = meta_x1 + sprite->offset.x;
        world_coord x2 = x1 + 7;
        uint8_t y1 = meta_y1 + sprite->offset.y;
        uint8_t y2 = y1 + 7;

        Bounds box = {
            .x1 = x1,
            .x2 = x2,
            .y1 = y1,
            .y2 = y2,
        };

        sprite_set_dirty(dirty, &box);
    }
}

bool spritemanager_add_metasprite(MetaSprite* meta) {
    for (int i = 0; i < sprite_slots_count; i++) {
        if (!sprite_manager.sprite_slots[i]) {
            sprite_manager.sprite_slots[i] = meta;
            return true;
        }
    }
    return false;
}
