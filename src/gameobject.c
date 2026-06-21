#include "gameobject.h"

GameObject* go_pool[gameobject_count];

bool go_spawn(GameObject* go) {
    for (int i = 0; i < gameobject_count; i++) {
        if (!go_pool[i]) {
            // empty
            if (go->metasprite) {
                bool sprite = spritemanager_add_metasprite(go->metasprite);
                if (!sprite) {
                    return false;
                }
            }
            go_pool[i] = go;

            return true;
        }
    }
    return false;
}

void go_pool_update() {
    for (int i = 0; i < gameobject_count; i++) {
        if (go_pool[i]) {
            go_update(go_pool[i]);
        }
    }
}

void position_add_and_limit(Vector2* lhs, const Vector2* rhs) {
    lhs->x.raw += rhs->x.raw;
    lhs->y.raw += rhs->y.raw;

    if ((int16_t)lhs->x.position < 0) {
        lhs->x.raw = 0;
    }

    if ((int16_t)lhs->y.position < 0) {
        lhs->y.raw = 0;
    } else if (lhs->y.position > 239) {
        lhs->y.position = 239;
    }
}

void go_update_pos(GameObject* go) {
    position_add_and_limit(&go->pos, &go->velocity);
}


void go_update(GameObject* go) {
    if (go->metasprite) {
        display_set_dirty_meta(go->metasprite);
    }

    go_update_pos(go);

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
    
    if ((int16_t)new.position < 0) {
        new.raw = 0;
    }

    camera_coord camera = coord_world_to_camera(new.position, display_get_camera_pos());

    if (camera > 127) {
        return camera - 127;
    }
    return 0;
}
