#include "driverlib.h"

#include "audio.h"
#include "musics.h"


void debaunce() {
    volatile int i = 20000;
    while (i--);
}

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;
    __enable_interrupt();

    audio_data_init();


    audio_channel_tone_set(ONE, 2000);


    // chaves
    // s1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;
    //s2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    while(1) {
        if (!(P2IN & BIT1)) {
            // s1 Pressed
            TA1CCR0 *= 2;
            TA1CCR1 *= 2;
            debaunce();
        }
        if (!(P1IN & BIT1)) {
            // s2 Pressed
            TA1CCR0 /= 2;
            TA1CCR1 /= 2;
            debaunce();
        }

    }
}
