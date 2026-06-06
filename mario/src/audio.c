#include "audio.h"
#include "musics.h"

#include <msp430f5529.h>


audio_data audio_data_ref;

// Uses TA1, channels 0 and 1
// P2.0 output 1
// P2.4 output 2
void audio_data_init() {
    // Channel ONE
    TA1CTL = TASSEL__SMCLK | MC__UP | ID_0 | TACLR;
    TA1CCTL1 = OUTMOD_3;

    P2DIR |= BIT0;
    P2SEL |= BIT0;

    // Channel TWO
    TA2CTL = TASSEL__SMCLK | MC__UP | ID_0 | TACLR;
    TA2CCTL1 = OUTMOD_3;

    // Controller
    TB0CTL = TBSSEL__ACLK | CNTL__16 | MC__CONTINOUS | ID_0 | TBCLR;
    TB0CCTL1 = CCIE;
    // TB0CCTL2 = CCIE;

    P2DIR |= BIT4;
    P2SEL |= BIT4;


    audio_data_ref.one.music_current = mario_theme;
    audio_data_ref.one.note_index = 0;
    audio_data_ref.one.note_repetitions = 0;
}

void audio_channel_tone_set(channel_index index, useconds wave_lenght) {
    switch (index) {
        case ONE:
        TA1CCR0 = wave_lenght;
        TA1CCR1 = wave_lenght/2;
        break;
        case TWO:
        TA2CCR0 = wave_lenght;
        TA2CCR1 = wave_lenght/2;
        break;
        default:
        __no_operation();
    }
}

void audio_channel_music_set(channel_index index, music *music) {
    switch (index) {
        case ONE:
        audio_data_ref.one.music_current = *music;
        case TWO:
        audio_data_ref.two.music_current = *music;
    }
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void audio_isr() {
    channel_data* channel;
    switch (__even_in_range(TB0IV, 14)) {
        case TB0IV_TBCCR1:
        channel = &audio_data_ref.one;
        channel->note_index = channel->note_index + 1 >= channel->music_current.size ? 0 : channel->note_index + 1;
        audio_channel_tone_set(ONE, channel->music_current.notes[channel->note_index].wave_lenght);
        TBCCR1 = TBR + channel->music_current.notes[channel->note_index].duration;
        break;
        case TB0IV_TBCCR2:
        channel = &audio_data_ref.two;
        channel->note_index = channel->note_index + 1 >= channel->music_current.size ? 0 : channel->note_index + 1;
        audio_channel_tone_set(TWO, channel->music_current.notes[channel->note_index].wave_lenght);
        TBCCR2 = TBR + channel->music_current.notes[channel->note_index].duration;
        break;
        default:
        break;
    }
}
