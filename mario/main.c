#include <driverlib.h>

#include "audio.h"
#include "musics.h"
#include "display.h"
#include "ili9341.h"
#include "coordinates.h"

#define SMCLK_FREQUENCY 1048576
#define FRAME_TARGET 16666

#define WORK_LED_ON P1OUT |= BIT0
#define WORK_LED_OFF P1OUT &= ~BIT0

#define SLEEP_LED_ON P4OUT |= BIT7
#define SLEEP_LED_OFF P4OUT &= ~BIT7

void debaunce() {
    volatile int i = 20000;
    while (i--);
}

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;

    audio_data_init();

    audio_channel_tone_set(ONE, 2000);
    // audio_channel_tone_set(TWO, 2000);

    display_init();
    // display_test2();

    TA0CTL = TASSEL__SMCLK | MC__CONTINOUS | ID_0 | TACLR;
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

    int frame_start = 0;
    int delta_time = 0;
    volatile int frame_max = 0;

    const uint16_t colors[] = {
        0xF800, // Red
        0xFAA0, // Orange-red
        0xFD20, // Orange
        0xFF80, // Yellow-orange
        0xFFE0, // Yellow
        0xAFE0, // Yellow-green
        0x57E0, // Green
        0x07E8, // Green-cyan
        0x07FF, // Cyan
        0x055F, // Cyan-blue
        0x02BF, // Blue
        0x501F, // Blue-violet
        0xA81F, // Violet
        0xD81F, // Magenta
        0xF90F, // Pink
        0xF800, // Back to red
    };

    uint8_t scroll_count = 0;

    // __enable_interrupt();
    while(1) {
        // Frame start
        delta_time = TA0R;
        TA0CTL |= TACLR;
        frame_start = TA0R;

        SLEEP_LED_OFF;
        WORK_LED_ON;
        // Frame
        if (!(P2IN & BIT1)) {
            // s1 Pressed
            audio_channel_music_set(TWO, &teste_theme);
            debaunce();
        }
        if (!(P1IN & BIT1)) {
            // s2 Pressed
            TA1CCR0 /= 2;
            TA1CCR1 /= 2;
            debaunce();
        }

        lcd_cmd_vertical_scrolling_start_address(32+scroll_count);

        memory_coord coord = coord_camera_to_memory(255, scroll_count);
        lcd_cmd_page_set(coord, coord);
        lcd_send_command(MEMORY_WRITE);
        lcd_send_wdata_repeat(colors[scroll_count & 15], lcd_height);
        scroll_count += 1;

        //Frame end
        WORK_LED_OFF;
        SLEEP_LED_ON;

        int frame_end = TA0R;
        int frame_duration = frame_end - frame_start;
        frame_max = frame_duration > frame_max ? frame_duration : frame_max;
        if (frame_duration < FRAME_TARGET-500) {
            __bis_SR_register(LPM0_bits + GIE); // enter low power mode
        }
    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void wakeup_isr() {
    // its assumed that only TA0CCTL1 can generate interrupt
    TA0CCTL1 &= ~CCIFG;
    __bic_SR_register_on_exit(LPM0_bits); 
}
