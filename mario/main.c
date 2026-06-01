#include <driverlib.h>

#include "audio.h"
#include "musics.h"
#include "display.h"

#define SMCLK_FREQUENCY 1048576
#define FRAME_TARGET 16666

#define WORK_LED_ON P1OUT |= BIT0
#define WORK_LED_OFF P1OUT &= ~BIT0

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
    display_test2();

    TA0CTL = TASSEL__SMCLK | MC__CONTINOUS | ID_0 | TACLR;
    TA0CCR1 = FRAME_TARGET;
    
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

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
    int frame_max = 0;

    __enable_interrupt();
    while(1) {
        // Frame start
        delta_time = TA0R;
        TA0CTL |= TACLR;
        frame_start = TA0R;

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

        //Frame end
        WORK_LED_OFF;

        int frame_end = TA0R;
        int frame_duration = frame_end - frame_start;
        frame_max = frame_duration > frame_max ? frame_duration : frame_max;
        if (frame_duration < FRAME_TARGET-100) {
            // TA0CCTL1 |= CCIE;
            // __bis_SR_register(LPM0_bits + GIE); // enter low power mode
        }
    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void wakeup_isr() {
    // its assumed that only TA0CCTL1 can generate interrupt
    TA0CCTL1 &= ~CCIE;
    __bic_SR_register_on_exit(LPM0_bits); 
}
