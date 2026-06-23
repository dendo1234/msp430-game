#include "gameobject.h"
#include "render/map/metatilemap.h"

GameObjectManager go_manager;

bool go_spawn(GameObject* go) {
    for (int i = 0; i < gameobject_count; i++) {
        if (!go_manager.pool[i]) {
            // empty
            if (go->metasprite) {
                bool sprite = spritemanager_add_metasprite(go->metasprite);
                if (!sprite) {
                    return false;
                }
            }
            go_manager.pool[i] = go;

            return true;
        }
    }
    return false;
}

void go_pool_update() {
    for (int i = 0; i < gameobject_count; i++) {
        if (go_manager.pool[i]) {
            go_update(go_manager.pool[i]);
        }
    }
}

void position_add_and_limit(Vector2* lhs, const Vector2* rhs) {
    lhs->x.raw += rhs->x.raw;
    lhs->y.raw += rhs->y.raw;

    if ((int16_t)lhs->y.position < 0) {
        lhs->y.raw = 0;
    } else if (lhs->y.position > 239) {
        lhs->y.position = 239;
    }
}

void go_collision_tile(GameObject* go, int16_t delta_x, int8_t delta_y) {
    Rect* box = &go->metasprite->box;

    world_coord metatile_x1 = box->x >> 4;
    world_coord metatile_x2 = (box->x + box->w-1) >> 4;
    uint8_t metatile_y1 = box->y >> 4;
    uint8_t metatile_y2 = (box->y + box->h-1) >> 4;

    go->isGrounded = false;
    if (delta_y > 0) {
        // check bottom
        for (int i = metatile_x1; i <= metatile_x2; i++) {
            Metatile meta = metamap_metatile_get2(go_manager.colision_map, i, metatile_y2);
            if (meta > META_SKY) {
                go->pos.y.position = go->pos.y.position & ~0xf; // go up
                go->isGrounded = true;
                break;
            }
        }
    } else if (delta_y < 0) {
        // check up
        for (int i = metatile_x1; i <= metatile_x2; i++) {
            Metatile meta = metamap_metatile_get2(go_manager.colision_map, i, metatile_y1);
            if (meta > META_SKY) {
                go->pos.y.position = (go->pos.y.position + 16) & ~0xf; // go down
                break;
            }
        }
    }

    metatile_y1 = go->pos.y.position >> 4;
    metatile_y2 = (go->pos.y.position + box->h-1) >> 4;

    if (delta_x > 0) {
        // check right
            for (int i = metatile_y1; i <= metatile_y2; i++) {
            Metatile meta = metamap_metatile_get2(go_manager.colision_map, metatile_x2, i);
            if (meta > META_SKY) {
                go->pos.x.position = go->pos.x.position & ~0xf; // go left
                break;
            }
        }
    } else if (delta_x < 0) {
        // check left
        for (int i = metatile_y1; i <= metatile_y2; i++) {
            Metatile meta = metamap_metatile_get2(go_manager.colision_map, metatile_x1, i);
            if (meta > META_SKY) {
                go->pos.x.position = (go->pos.x.position + 16) & ~0xf; // go right
                break;
            }
        }
    }
}

void go_update_pos(GameObject* go) {
    position_add_and_limit(&go->pos, &go->velocity);
}


void go_update(GameObject* go) {
    if (!go) {
        return;
    }

    if (go->metasprite) {
        display_set_dirty_meta(go->metasprite);
    }

    world_coord old_x = go->pos.x.position;
    uint8_t old_y = go->pos.y.position;
    
    go->velocity.y.raw += 0x10000;

    if (go->velocity.y.raw > 0x40000) {
        go->velocity.y.raw = 0x40000;
    }

    go->pos.x.raw += go->velocity.x.raw;
    go->pos.y.raw += go->velocity.y.raw;

    if (go->pos.x.position < display_get_camera_pos()) {
        go->pos.x.position = display_get_camera_pos();
    }

    int16_t delta_x = go->pos.x.position - old_x;
    int8_t delta_y = go->pos.y.position - old_y;

    go->metasprite->box.x = go->pos.x.position;
    go->metasprite->box.y = go->pos.y.position;
    go_collision_tile(go, delta_x, delta_y);

    go->metasprite->box.x = go->pos.x.position;
    go->metasprite->box.y = go->pos.y.position;

    if (go->metasprite) {
        display_set_dirty_meta(go->metasprite);
    }
}

uint8_t go_calculate_camera_delta(GameObject* mario) {
    PosX new = {
        .raw = mario->pos.x.raw + mario->velocity.x.raw
    };
    
    if ((int16_t)(new.position - display_get_camera_pos()) < 0) {
        new.position = display_get_camera_pos();
    }

    camera_coord camera = coord_world_to_camera(new.position, display_get_camera_pos());

    if (camera > 127) {
        return camera - 127;
    }
    return 0;
}
