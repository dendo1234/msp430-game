#include "mario.h"

GameObject go_mario = {
    .metasprite = &metasprite_mario,
};

void mario_move(ADC_result joystick, uint8_t button_b) {
    // go_mario.velocity.x.raw = (int32_t)joystick.x * (0x500l); // 2.5 pixels per frame
    // go_mario.velocity.y.raw = (int32_t)joystick.y * (0x500l); // 2.5 pixels per frame

    if (joystick.y < -115) {
        if (go_mario.isGrounded) {
            go_mario.velocity.y.raw = -0x70000;
        } else {
            go_mario.velocity.y.raw -= 0xC000;
        }
    }

    if (go_mario.velocity.x.raw > 0x4000) {
        go_mario.velocity.x.raw -= 0x4000;
    } else if (go_mario.velocity.x.raw < -0x4000) {
        go_mario.velocity.x.raw += 0x4000;
    } else {
        go_mario.velocity.x.raw = 0;
    }

    if (joystick.x > 0 == go_mario.velocity.x.raw > 0) {
        go_mario.velocity.x.raw += (int32_t)joystick.x * (0x100l); // 2.5 pixels per frame
    } else {
        go_mario.velocity.x.raw += (int32_t)joystick.x * (0x280l); // 2.5 pixels per frame
    }
    
    int32_t max_speed = 0x10000l;
    if (button_b) {
        max_speed = 0x28000l;
    }

    if (go_mario.velocity.x.raw > max_speed) {
        go_mario.velocity.x.raw = max_speed;
    }
    if (go_mario.velocity.x.raw < -max_speed) {
        go_mario.velocity.x.raw = -max_speed;
    }

}
