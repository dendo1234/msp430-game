#include "audio.h"
#include "musics.h"

#include "msp430f5529.h"

channel_data audio_data_ref;

void audio_data_init() {
    // Channel ONE
    TA1CTL = TASSEL__SMCLK | MC__UP | ID_0 | TAIE | TACLR;
    TA1CCTL0 = CCIE;
    TA1CCTL1 = OUTMOD_3;

    P2DIR |= BIT0;
    P2SEL |= BIT0;

    // Channel TWO
    TA2CTL = TASSEL__SMCLK | MC__UP | ID_0 | TAIE | TACLR;
    TA2CCTL0 = CCIE;
    TA2CCTL1 = OUTMOD_3;

    // P2DIR |= BIT4;
    // P2SEL |= BIT4;


    audio_data_ref.music_current = mario_theme;
    audio_data_ref.note_index = 0;
    audio_data_ref.note_repetitions = 0;
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
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void audio_channel_1_ISR() {
    audio_data_ref.note_repetitions++;
    if (audio_data_ref.note_repetitions >= audio_data_ref.music_current.notes[audio_data_ref.note_index].repetitions) {
        audio_data_ref.note_repetitions = 0;
        audio_data_ref.note_index = audio_data_ref.note_index + 1 >= audio_data_ref.music_current.size ? 0 : audio_data_ref.note_index + 1;
        audio_channel_tone_set(ONE, audio_data_ref.music_current.notes[audio_data_ref.note_index].wave_lenght);
    }
    TA1CTL &= ~TAIFG;
    return;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void audio_channel_2_ISR() {
    audio_data_ref.note_repetitions++;
    if (audio_data_ref.note_repetitions >= audio_data_ref.music_current.notes[audio_data_ref.note_index].repetitions) {
        audio_data_ref.note_repetitions = 0;
        audio_data_ref.note_index = audio_data_ref.note_index + 1 >= audio_data_ref.music_current.size ? 0 : audio_data_ref.note_index + 1;
        audio_channel_tone_set(TWO, audio_data_ref.music_current.notes[audio_data_ref.note_index].wave_lenght);
    }
    TA2CTL &= ~TAIFG;
    return;
}