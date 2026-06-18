#include "sprite.h"
#include "display.h"

SpriteManager sprite_manager;


void sprite_render_dirty8x8(uint16_t* destination, world_coord tile_x1, uint8_t y) {

    world_coord tile_x2 = tile_x1 + 7;
    uint8_t tile_y1 = y & ~0x7;
    uint8_t tile_y2 = tile_y1 + 7;

    Tile tile = metamap_tile_get(sprite_manager.background, tile_x1, tile_y1);

    tile_full_copy(tile, destination);

    // render

    for (int i = 0; i < sprite_slots_count; i++) {
        Sprite* sprite = sprite_manager.sprite_slots[i];

        if (!sprite->render) {
            continue;
        }

        Box* box = &sprite->box;

        Box dst_box;
        Box src_box;

        if (box->x1 >= tile_x1 && box->x1 <= tile_x2) {
            dst_box.x1 = box->x1 & 0x7;
            dst_box.x2 = 7;

            src_box.x1 = 0;
            src_box.x2 = 7 - (box->x1 & 0x7);
        } else if (box->x2 >= tile_x1 && box->x2 <= tile_x2) {
            dst_box.x1 = 0;
            dst_box.x2 = box->x2 & 0x7;

            src_box.x1 = 7 - (box->x2 & 0x7);
            src_box.x2 = 7;
        } else {
            continue;
        }

        if (box->y1 >= tile_y1 && box->y1 <= tile_y2) {
            dst_box.y1 = box->y1 & 0x7;
            dst_box.y2 = 7;

            src_box.y1 = 0;
            src_box.y2 = 7 - (box->y1 & 0x7);
        } else if (box->y2 >= tile_y1 && box->y2 <= tile_y2) {
            dst_box.y1 = 0;
            dst_box.y2 = box->y2 & 0x7;

            src_box.y1 = 7 - (box->y2 & 0x7);
            src_box.y2 = 7;
        } else {
            continue;
        }

        sprite_ret_copy(destination, sprite, &src_box, &dst_box);
    }
}

void sprite_ret_copy(uint16_t* destination, Sprite* sprite, Box* src_box, Box* dst_box) {
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

void sprite_set_dirty(uint8_t dirty[4][30], Box* box) {
    world_coord camera_pos = display_get_camera_pos();
    uint8_t y1 = box->y1 >> 3;
    uint8_t y2 = box->y2 >> 3;
    if (coord_cull(box->x1, camera_pos)) {
        camera_coord x = coord_world_to_camera(box->x1, camera_pos);
        uint8_t tile_x = x >> 3;

        dirty[tile_x >> 3][y1] |= 1 << (tile_x & 0x7);
        dirty[tile_x >> 3][y2] |= 1 << (tile_x & 0x7);
    }

    if (coord_cull(box->x2, camera_pos)) {
        camera_coord x = coord_world_to_camera(box->x2, camera_pos);
        uint8_t tile_x = x >> 3;

        dirty[tile_x >> 3][y1] |= 1 << (tile_x & 0x7);
        dirty[tile_x >> 3][y2] |= 1 << (tile_x & 0x7);
    }
}
