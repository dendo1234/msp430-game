#pragma once
#include <stdint.h>
#include "coordinates.h"
#include "render/sprite.h"
#include "render/display.h"
#include "render/sprite_pool.h"

typedef union {
    int32_t raw;
    struct {
        uint16_t subpixel; 
        world_coord position;
    };
} PosX;

typedef union {
    int32_t raw;
    struct {
        uint16_t subpixel; 
        uint8_t position;
        uint8_t padding;
    };
} PosY;

typedef struct {
    PosX x;
    PosY y;
} Vector2;

typedef struct {
    Vector2 pos;
    Vector2 velocity;
    MetaSprite* metasprite;
} GameObject;

static const uint8_t gameobject_count = 8;

bool go_spawn(GameObject* go);
void go_pool_update();
void go_update(GameObject* go);

uint8_t go_calculate_camera_delta(GameObject* mario);
