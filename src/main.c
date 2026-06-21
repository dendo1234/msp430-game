#include <driverlib.h>

#include "adc.h"
#include "audio.h"
#include "clocks.h"
#include "coordinates.h"
#include "gameobject.h"
#include "gameobject_pool.h"
#include "musics.h"
#include "render/display.h"
#include "render/sprite_pool.h"

const uint8_t MAIN_CLK_DIVIDER_POWER = 3; // 2^x
const uint16_t FRAME_TARGET = (SMCLK_FREQUENCY/(1 << MAIN_CLK_DIVIDER_POWER))*0.016667; //60 fps

#define WORK_LED_ON P1OUT |= BIT0
#define WORK_LED_OFF P1OUT &= ~BIT0

#define SLEEP_LED_ON P4OUT |= BIT7
#define SLEEP_LED_OFF P4OUT &= ~BIT7

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;

    // audio_data_init();

    display_init();

    adc_init();

    TA0CTL = TASSEL__SMCLK | MC__CONTINOUS | (ID__1 * MAIN_CLK_DIVIDER_POWER) | TACLR;
    TA0CCTL1 = CCIE;
    TA0CCR1 = FRAME_TARGET;
    
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    // chaves
    // s1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;
    //s2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    uint16_t frame_start = 0;
    int delta_time = 0;
    volatile uint16_t frame_max = 0;

    // display_render_all(tilemap_color_picker);
    // PMM_setVCore(PMM_CORE_LEVEL_2);
    // overclock();

    sprite_manager.background = &metamap1;
    go_spawn(&go_mario);

    __enable_interrupt();
    while(1) {
        // Frame start
        delta_time = TA0R;
        TA0CTL |= TACLR;
        TA0CTL &= ~TAIFG;
        frame_start = TA0R;

        WORK_LED_ON;
        // Frame
        adc_start_conversion();

        if (!(P2IN & BIT1)) {
            // s1 Pressed
            audio_channel_music_set(TWO, &teste_theme);
        }
        if (!(P1IN & BIT1)) {
            // s2 Pressed

        }

        ADC_result joystick = adc_read();

        go_mario.velocity.x.raw = (int32_t)joystick.x * (0x500l); // 2.5 pixels per frame
        go_mario.velocity.y.raw = (int32_t)joystick.y * (0x500l); // 2.5 pixels per frame

        uint8_t camera_delta = go_calculate_camera_delta(&go_mario);
        display_camera_add(camera_delta);

        go_pool_update();

        display_render_new_columns_metatilemap();
        display_set_dirty_meta(&metasprite_mario);
        display_render_dirty_sprites();

        //Frame end
        WORK_LED_OFF;

        uint16_t frame_end = TA0R;
        uint16_t frame_duration = frame_end - frame_start;
        frame_max = frame_duration > frame_max ? frame_duration : frame_max;
        if (frame_duration < FRAME_TARGET-100 && !(TA0CTL & TAIFG)) {
            SLEEP_LED_ON;
            __bis_SR_register(LPM0_bits + GIE); // enter low power mode
        }
    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void wakeup_isr() {
    // its assumed that only TA0CCTL1 can generate interrupt
    SLEEP_LED_OFF;
    TA0CCTL1 &= ~CCIFG;
    __bic_SR_register_on_exit(LPM0_bits); 
}
