#include <driverlib.h>

#include "audio.h"
#include "musics.h"
#include "display.h"
#include "ili9341.h"
#include "coordinates.h"
#include "clocks.h"
#include "map/tilemap.h"

#define FRAME_TARGET ((SMCLK_FREQUENCY/2 /* timer divider */)*0.016667) //60 fps

#define WORK_LED_ON P1OUT |= BIT0
#define WORK_LED_OFF P1OUT &= ~BIT0

#define SLEEP_LED_ON P4OUT |= BIT7
#define SLEEP_LED_OFF P4OUT &= ~BIT7

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;

    // audio_data_init();

    display_init();

    TA0CTL = TASSEL__SMCLK | MC__CONTINOUS | ID__1 | TACLR;
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

    bool render_mode = false;

    __enable_interrupt();
    while(1) {
        // Frame start
        delta_time = TA0R;
        TA0CTL |= TACLR;
        TA0CTL &= ~TAIFG;
        frame_start = TA0R;

        WORK_LED_ON;
        // Frame
        if (!(P2IN & BIT1)) {
            // s1 Pressed
            audio_channel_music_set(TWO, &teste_theme);
        }
        if (!(P1IN & BIT1)) {
            // s2 Pressed
            render_mode = !render_mode;

        }

        display_camera_add(3);

        if (render_mode) {
            display_render_new_columns(tilemap_color_picker);
        } else {
            display_render_new_columns_metatilemap();
        }
        // display_test2();


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
