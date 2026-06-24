#include "gameobject.h"
#include "gameobject_pool.h"

static const uint8_t spawn_count = 32;

typedef struct GoSpawner {
    uint8_t spawned;
    Offset spawn_position[spawn_count];
    GameObject* list[spawn_count];
} GoSpawner;



void spwaner_check(world_coord camera_pos);
