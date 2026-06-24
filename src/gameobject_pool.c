#include "gameobject_pool.h"

void koopa_update(GameObject* go) {
    go->velocity.x.raw = -0x10000;
}

GameObject go_koopa_template = {
    .box = {
        .x = 280,
        .y = 40,
        .w = 16,
        .h = 16
    },
    .metasprite = &metasprite_mario,
    .isAlive = true,
    .render = true,
    .custom_function = koopa_update,
    .late_update = koopa_update
};
