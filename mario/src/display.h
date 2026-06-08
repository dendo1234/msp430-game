// display == renderer
#pragma once
#include <stdint.h>
#include "coordinates.h"

typedef struct {
    world_coord camera_pos;
    uint8_t scroll_count;
    uint8_t new_columns;
    uint8_t dirty_8x8[4*30]; // each bit symbolizes that the renderer should update the given 8x8
} display_data;

typedef uint16_t color;

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
void display_render_all(color_picker fun);

color test_color_picker(world_coord x, uint8_t y); 
