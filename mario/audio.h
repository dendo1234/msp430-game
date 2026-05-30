#pragma once

typedef int useconds;

typedef struct note {
    useconds wave_lenght;
    int repetitions;
} note;

#define NOTE(_frequency, _duration_rounded_s) (const note){ .wave_lenght = 1000000.0/_frequency, .repetitions = (int)(_duration_rounded_s/(1.0/_frequency))}
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

__interrupt void audio_channel_1_ISR();
