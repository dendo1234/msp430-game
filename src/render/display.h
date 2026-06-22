// display == renderer
#pragma once
#include <stdint.h>
#include "coordinates.h"
#include "sprite.h"

typedef struct {
    world_coord camera_pos;
    uint8_t scroll_count;
    uint8_t new_columns;
    color buffer[2][256];
    uint8_t current_buffer;
    // each bit means that the renderer should rerender the world tile at the given index + camera offset
    //
    // One consequence is that since there can be more than 32 world tiles in view on the horizontal axis
    // (31 "full tiles" + 2 partial at the begining and end)
    // is that the last tile in view horizontaly is not tracked
    uint8_t dirty_8x8[4][30];
} display_data;


typedef color (*color_picker)(world_coord x, uint8_t y);

void display_init();
void display_draw_solid_frame(uint16_t color);
void display_test1();
void display_test2();
void display_test3();

void display_reset();
void display_camera_add(uint16_t x);
void display_render_new_columns(color_picker fun);
void display_render_new_columns16(color_picker fun);
void display_render_all();

color* display_buffer_get();

void display_render_new_columns_metatilemap();
void display_render_dirty_sprites();
world_coord display_get_camera_pos();

color test_color_picker(world_coord x, uint8_t y); 

void display_set_dirty(Bounds* box);
void display_set_dirty_meta(MetaSprite* meta);
