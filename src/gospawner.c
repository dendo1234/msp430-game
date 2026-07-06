#include "gospawner.h"

GoSpawner spawner = {
    .spawn_position = {
        { 100, 32 },
        { 200, 32 },
        { 300, 32 },
        { 400, 32 },
        { 500, 32 },
        { 600, 32 },
        { 700, 32 },
        { 800, 32 },
        { 900, 32 },
        { 1000, 32 },
        { 1100, 32 },
        { 1200, 32 },
        { 1300, 32 },
        { 1400, 32 },
        { 1500, 32 },
        { 1600, 32 },
        { 1700, 32 },
        { 1800, 32 },
        { 1900, 32 },
        { 2000, 32 },
        { 2100, 32 },
        { 2200, 32 },
        { 2300, 32 },
        { 2400, 32 },
        { 2500, 32 },
        { 2600, 32 },
        { 2700, 32 },
        { 2800, 32 },
        { 2900, 32 },
        { 3000, 32 },
        { 3100, 32 },
        { 3200, 32 },
    },
    .list = {
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
        &go_koopa_template,
    }
};

void spwaner_check(world_coord camera_pos) {
    while (spawner.spawned < spawn_count) {
        if (spawner.spawn_position[spawner.spawned].x > camera_pos+256) {
            break;
        }
        spawner.list[spawner.spawned]->box.x = spawner.spawn_position[spawner.spawned].x;
        spawner.list[spawner.spawned]->box.y = spawner.spawn_position[spawner.spawned].y;
        spawner.list[spawner.spawned]->pos.x.position = spawner.spawn_position[spawner.spawned].x;
        spawner.list[spawner.spawned]->pos.y.position = spawner.spawn_position[spawner.spawned].y;
        go_spawn(spawner.list[spawner.spawned]);
        spawner.spawned++;
    }
}
