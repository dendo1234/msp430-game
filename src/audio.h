#pragma once

#include <stdint.h>
#include "clocks.h"


#define TIMER_A_CLK_FREQUENCY SMCLK_FREQUENCY
#define TIMER_B_CLK_FREQUENCY ACLK_FREQUENCY

typedef uint16_t useconds;
typedef uint16_t timer_ticks;

typedef struct note {
    useconds wave_lenght;
    timer_ticks duration;
} note;

#define double_to_ticks(db) ((timer_ticks)((double)db*TIMER_B_CLK_FREQUENCY))
#define NOTE(_frequency, _duration_rounded_s) (const note){ .wave_lenght = TIMER_A_CLK_FREQUENCY/_frequency, .duration = double_to_ticks(_duration_rounded_s)}
#define NOTE_SILENCE(_duration_rounded_s) NOTE(20000, _duration_rounded_s)

typedef struct {
    note const * notes;
    int size;
} music;

typedef struct {
    music music_current;
    music music_next;
    int note_index;
    int note_repetitions;
} channel_data;

typedef enum {
    ONE, // main
    TWO
} channel_index;

typedef struct {
    channel_data one;
    channel_data two;
} audio_data;


void audio_data_init();

void audio_channel_tone_set(channel_index audio_channel, useconds wave_lenght);
void audio_channel_music_set(channel_index audio_channel, music* music);

__interrupt void audio_isr();
