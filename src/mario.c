#include "mario.h"
#include "audio.h"
#include "musics.h"
#include "adc.h"

#include <driverlib.h>

void mario_move(GameObject* go) {
    // go_mario.velocity.x.raw = (int32_t)joystick.x * (0x500l); // 2.5 pixels per frame
    // go_mario.velocity.y.raw = (int32_t)joystick.y * (0x500l); // 2.5 pixels per frame
 
    display_set_dirty_meta(go);

    ADC_result joystick = adc_read();
    if (!(P1IN & BIT2)) {
        joystick.y = -128;
    }
 
    if (joystick.y < -115) {
        if (go->isGrounded) {
            go->velocity.y.raw = -0x70000;
            audio_channel_music_set(TWO, &music_jump);
        } else {
            go->velocity.y.raw -= 0xC000;
        }
    }

    if (go->velocity.x.raw > 0x4000) {
        go->velocity.x.raw -= 0x4000;
    } else if (go->velocity.x.raw < -0x4000) {
        go->velocity.x.raw += 0x4000;
    } else {
        go->velocity.x.raw = 0;
    }

    if (joystick.x > 0 == go->velocity.x.raw > 0) {
        go->velocity.x.raw += (int32_t)joystick.x * (0x100l); // 2.5 pixels per frame
    } else {
        go->velocity.x.raw += (int32_t)joystick.x * (0x280l); // 2.5 pixels per frame
    }
    
    int32_t max_speed = 0x10000l;
    if (!(P1IN & BIT3)) {
        max_speed = 0x28000l;
    }

    if (go->velocity.x.raw > max_speed) {
        go->velocity.x.raw = max_speed;
    }
    if (go->velocity.x.raw < -max_speed) {
        go->velocity.x.raw = -max_speed;
    }


    uint8_t camera_delta = go_calculate_camera_delta(go);
    display_camera_add(camera_delta);


}

void mario_late(GameObject* go) {
    display_set_dirty_meta(go);
}

GameObject go_mario = {
    .box = {
        .x = 280,
        .y = 40,
        .w = 16,
        .h = 24
    },
    .isAlive = true,
    .render = true,
    .metasprite = &metasprite_koopa,
    .custom_function = mario_move,
    .late_update = mario_late
};
