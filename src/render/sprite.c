#include "sprite.h"
#include "display.h"

SpriteManager manager;

Sprite8x8 mario8x8 = {
    0xB184,0xB184,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0xB184,0xB184,0xB184,0xB184,0xB184,0x0000,0x0000,0x0000,
    0xECE4,0x6B60,0xECE4,0x0000,0x0000,0x0000,0x0000,0x0000,
    0xECE4,0x6B60,0xECE4,0xECE4,0xECE4,0x0000,0x0000,0x0000,
    0xECE4,0xECE4,0x6B60,0xECE4,0xECE4,0xECE4,0x0000,0x0000,
    0xECE4,0x6B60,0x6B60,0x6B60,0x6B60,0x0000,0x0000,0x0000,
    0xECE4,0xECE4,0xECE4,0xECE4,0x0000,0x0000,0x0000,0x0000,
    0x6B60,0x6B60,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

Sprite mario = {
    .data = &mario8x8,
    .box = {
        .x1 = 260,
        .x2 = 267,
        .y1 = 20,
        .y2 = 27,
    },
    .flip = false,
};



void sprite_find_sprites_in_tile(uint8_t index_x, uint8_t index_y) {

}

void sprite_render_dirty8x8(uint16_t* destination, uint8_t index_x, uint8_t index_y) {
    uint8_t index_world_x = index_x + (display_get_camera_pos() >> 3);

    // Sprite* sprites_overlapping[sprite_slots_count];
    // uint8_t sprites_overlapping_count = 0;
    // for (int i = 0; i < sprite_slots_count; i++) {
    //     // check if any ovelaps the area
    //     Box* box = &manager.sprite_slots[i].box;
    //     if (sprite_check_inside_8x8(box, index_world_x, index_y)) {
    //         sprites_overlapping[sprites_overlapping_count] = &manager.sprite_slots[i];
    //         sprites_overlapping_count++;
    //     }
    // }

    uint8_t tile_y1 = index_y * 8;
    world_coord tile_x1 = index_world_x << 3;
    uint8_t tile_y2 = tile_y1 + 7;
    world_coord tile_x2 = tile_x1 + 7;

    Tile tile = metamap_tile_get(manager.background, tile_x1, tile_y1);

    tile_full_copy(tile, destination);

    // render

    for (int i = 0; i < sprite_slots_count; i++) {
        Sprite* sprite = &manager.sprite_slots[i];

        if (!sprite->render) {
            continue;
        }

        Box* box = &sprite->box;

        Box src_box;

        if (box->x1 >= tile_x1 && box->x1 <= tile_x2) {
            src_box.x1 = box->x1 & 0x7;
            src_box.x2 = 7;
        } else if (box->x2 >= tile_x1 && box->x2 <= tile_x2) {
            src_box.x1 = 0;
            src_box.x2 = box->x2 & 0x7;
        } else {
            continue;
        }

        if (box->y1 >= tile_y1 && box->y1 <= tile_y2) {
            src_box.y1 = box->y1 & 0x7;
            src_box.y2 = 7;
        } else if (box->y2 >= tile_y1 && box->y2 <= tile_y2) {
            src_box.y1 = 0;
            src_box.y2 = box->y2 & 0x7;
        } else {
            continue;
        }

        sprite_ret_copy(destination, sprite, &src_box);
    }
}

void sprite_ret_copy(uint16_t* destination, Sprite* sprite, Box* src_box) {
    const color* data = (*sprite->data);

    destination += src_box->x1;
    destination += src_box->y1*8;

    data += src_box->x1;
    data += src_box->y1*8;

    uint8_t count_x = src_box->x2 - src_box->x1 + 1;
    uint8_t count_y = src_box->y2 - src_box->y1 + 1;

    uint8_t stride = 8 - count_x;

    while (count_y)
    {
        int i = count_x;
        while(i) {
            *destination++ = *data++;
            i--;

        }
        destination += stride;
        data += stride;
    }
};

#include "map/tileset.h"

// void sprite_render_column(uint16_t* destination, world_coord x, uint8_t y) {
//     manager.sprite_slots[0] = mario;
//     manager.used_slots = 1;

//     Metatile meta = metamap_metatile_get(manager.background, x, y);
//     Tile tile = metatile_tile_get(meta, x, y);

//     uint8_t offset_x = x & 0x7;

//     const color* q = tileset_main[tile] + offset_x;

//     for (int i = 0; i < 8; i++) {
//         *destination++ = *q;
//         q += 8;
//     }

//     destination -= 8;

//     Sprite* sprite = &manager.sprite_slots[0];
//     uint8_t sprite_mask = manager.used_slots;
//     do {
//         if (sprite_mask & 1) {
//             // check bounds
//             Box box = sprite->box;
//             if (x >= box.x2 && x <= box.x2 && y >= box.y1 && y <= box.y2) {
//                 const color* data = (*sprite->data) + (x - box.x1) + (y - box.y1)*8; 

//                 while (y <= box.y2) {
//                     if (*data) {
//                         *destination = *data;
//                     }
//                     destination++;
//                     data += 8;
//                     y++;
//                 }
//             }
//         }
//         sprite++;
//         sprite_mask = sprite_mask >> 1;
//     } while (sprite_mask);
// }

// bool sprite_check_inside(Box* box, world_coord x, uint8_t y) {
//     if (y >= box->y && y < box->y + box->h) {
//         if (x >= box->x && x < box->x + box->w) {
//             return true;
//         }
//     }
//     return false;
// }

// bool sprite_check_inside(Box* box, uint16_t index_x, uint8_t index_y) {
//     box->x1
// }

bool sprite_check_inside_8x8(Box* box, uint8_t x, uint8_t y) {
    bool one = x >= box->x1 || x <= box->x2;
    bool two = y >= box->y1 || y <= box->y2;
    return one && two;
}
