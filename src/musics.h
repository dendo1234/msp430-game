#pragma once
#include "audio.h"

// note frequencies
#define B5 988
#define A5 880
#define G5 784
#define F5 698
#define E5 659
#define D5 587
#define C5 523
#define B4 494
#define A4 440
#define G4 392
#define F4 349
#define E4 330
#define D4 294
#define C4 262

#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784

static const note mario_intro[] = {
    // Measure 1 - The classic syncopated riff (1.20 seconds total)
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.05),  // E5 (Eighth)
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.05),  // E5 (Eighth)
    NOTE_SILENCE(0.15),                       // Rest (Eighth)
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.05),  // E5 (Eighth)
    NOTE_SILENCE(0.15),                       // Rest (Eighth)
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.05),  // C5 (Eighth)
    NOTE(NOTE_E5, 0.20), NOTE_SILENCE(0.10),  // E5 (Quarter note - held slightly longer)

    // Measure 2 - The resolution (1.20 seconds total)
    NOTE(NOTE_G5, 0.20), NOTE_SILENCE(0.40),  // G5 (Quarter note followed by quarter rest)
    NOTE(NOTE_G4, 0.20), NOTE_SILENCE(0.40)   // G4 (Low G quarter note followed by quarter rest)
};

// Size helper to easily assign to your `music` struct later
#define MARIO_INTRO_SIZE (sizeof(mario_intro) / sizeof(mario_intro[0]))


static music music_intro = {
    .notes = mario_intro,
    .size = MARIO_INTRO_SIZE,
};

// Additional Pitch Frequencies (Hz) for Theme A
#define NOTE_E4  330
#define NOTE_A4  440
#define NOTE_AS4 466  // Bb4 / A#4
#define NOTE_B4  494
#define NOTE_D5  587
#define NOTE_F5  698
#define NOTE_A5  880

static const note mario_theme_a[] = {
    NOTE(523, 0.20), NOTE_SILENCE(0.25),  // C5
    NOTE(392, 0.10), NOTE_SILENCE(0.20),  // G4
    NOTE(330, 0.20), NOTE_SILENCE(0.25),  // E4
    NOTE(440, 0.20), NOTE_SILENCE(0.10),  // A4
    NOTE(494, 0.20), NOTE_SILENCE(0.10),  // B4
    NOTE(466, 0.10), NOTE_SILENCE(0.05),  // Bb4
    NOTE(440, 0.20), NOTE_SILENCE(0.10),  // A4
    NOTE(392, 0.10), NOTE_SILENCE(0.05),  // G4
    NOTE(659, 0.10), NOTE_SILENCE(0.05),  // E
    NOTE(784, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(880, 0.20), NOTE_SILENCE(0.10),  // A5
    NOTE(698, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(784, 0.20), NOTE_SILENCE(0.10),  // G5
    NOTE(659, 0.20), NOTE_SILENCE(0.10),  // E5
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(587, 0.10), NOTE_SILENCE(0.05),  // D5
    NOTE(494, 0.20), NOTE_SILENCE(0.25),  // B4

    NOTE(523, 0.20), NOTE_SILENCE(0.25),  // C5
    NOTE(392, 0.10), NOTE_SILENCE(0.20),  // G4
    NOTE(330, 0.20), NOTE_SILENCE(0.25),  // E4
    NOTE(440, 0.20), NOTE_SILENCE(0.10),  // A4
    NOTE(494, 0.20), NOTE_SILENCE(0.10),  // B4
    NOTE(466, 0.10), NOTE_SILENCE(0.05),  // Bb4
    NOTE(440, 0.20), NOTE_SILENCE(0.10),  // A4
    NOTE(392, 0.10), NOTE_SILENCE(0.05),  // G4
    NOTE(659, 0.10), NOTE_SILENCE(0.05),  // E
    NOTE(784, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(880, 0.20), NOTE_SILENCE(0.10),  // A5
    NOTE(698, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(784, 0.20), NOTE_SILENCE(0.10),  // G5
    NOTE(659, 0.20), NOTE_SILENCE(0.10),  // E5
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(587, 0.10), NOTE_SILENCE(0.05),  // D5
    NOTE(494, 0.20), NOTE_SILENCE(0.25),  // B4
};

// Size helper for your `music` struct
#define MARIO_THEME_A_SIZE (sizeof(mario_theme_a) / sizeof(mario_theme_a[0]))

static music music_theme_a = {
    .notes = mario_theme_a,
    .size = MARIO_THEME_A_SIZE
};

// Additional Pitch Frequencies (Hz) for Theme B
#define NOTE_GS4 415  // G#4 / Ab4
#define NOTE_DS5 622  // D#5 / Eb5
#define NOTE_FS5 740  // F#5 / Gb5
#define NOTE_C6  1047

static const note mario_theme_b[] = {
    // Measure 11 - The descending chromatic walk
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_G5, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(NOTE_FS5, 0.10), NOTE_SILENCE(0.05), // F#5
    NOTE(NOTE_F5, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(NOTE_DS5, 0.10), NOTE_SILENCE(0.20), // D#5
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.20),  // E5

    // Measure 12 - The arpeggio bounce
    NOTE(NOTE_GS4, 0.10), NOTE_SILENCE(0.05), // G#4
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.20),  // C5
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(NOTE_D5, 0.10), NOTE_SILENCE(0.20),  // D5

    // Measure 13 - Repeat of the descending walk
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_G5, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(NOTE_FS5, 0.10), NOTE_SILENCE(0.05), // F#5
    NOTE(NOTE_F5, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(NOTE_DS5, 0.10), NOTE_SILENCE(0.20), // D#5
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.20),  // E5

    // Measure 14 - The high peak
    NOTE(NOTE_C6, 0.10), NOTE_SILENCE(0.20),  // C6
    NOTE(NOTE_C6, 0.10), NOTE_SILENCE(0.05),  // C6
    NOTE(NOTE_C6, 0.20), NOTE_SILENCE(0.40),  // C6 (Held slightly longer, followed by long rest)

    // Measure 15 & 16 - Walk and arpeggio repeated
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_G5, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(NOTE_FS5, 0.10), NOTE_SILENCE(0.05), // F#5
    NOTE(NOTE_F5, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(NOTE_DS5, 0.10), NOTE_SILENCE(0.20), // D#5
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.20),  // E5
    
    NOTE(NOTE_GS4, 0.10), NOTE_SILENCE(0.05), // G#4
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.20),  // C5
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(NOTE_D5, 0.10), NOTE_SILENCE(0.20),  // D5

    // Measure 17 & 18 - The bridge resolution (Eb5 -> D5 -> C5)
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_DS5, 0.20), NOTE_SILENCE(0.10), // Eb5 (Same frequency as D#5)
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_D5, 0.10), NOTE_SILENCE(0.05),  // D5
    NOTE_SILENCE(0.30),                       // Rest
    NOTE(NOTE_C5, 0.20), NOTE_SILENCE(0.10),   // C5

    // Measure 11 - The descending chromatic walk
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_G5, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(NOTE_FS5, 0.10), NOTE_SILENCE(0.05), // F#5
    NOTE(NOTE_F5, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(NOTE_DS5, 0.10), NOTE_SILENCE(0.20), // D#5
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.20),  // E5

    // Measure 12 - The arpeggio bounce
    NOTE(NOTE_GS4, 0.10), NOTE_SILENCE(0.05), // G#4
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.20),  // C5
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(NOTE_D5, 0.10), NOTE_SILENCE(0.20),  // D5

    // Measure 13 - Repeat of the descending walk
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_G5, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(NOTE_FS5, 0.10), NOTE_SILENCE(0.05), // F#5
    NOTE(NOTE_F5, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(NOTE_DS5, 0.10), NOTE_SILENCE(0.20), // D#5
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.20),  // E5

    // Measure 14 - The high peak
    NOTE(NOTE_C6, 0.10), NOTE_SILENCE(0.20),  // C6
    NOTE(NOTE_C6, 0.10), NOTE_SILENCE(0.05),  // C6
    NOTE(NOTE_C6, 0.20), NOTE_SILENCE(0.40),  // C6 (Held slightly longer, followed by long rest)

    // Measure 15 & 16 - Walk and arpeggio repeated
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_G5, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(NOTE_FS5, 0.10), NOTE_SILENCE(0.05), // F#5
    NOTE(NOTE_F5, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(NOTE_DS5, 0.10), NOTE_SILENCE(0.20), // D#5
    NOTE(NOTE_E5, 0.10), NOTE_SILENCE(0.20),  // E5
    
    NOTE(NOTE_GS4, 0.10), NOTE_SILENCE(0.05), // G#4
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.20),  // C5
    NOTE(NOTE_A4, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(NOTE_D5, 0.10), NOTE_SILENCE(0.20),  // D5

    // Measure 17 & 18 - The bridge resolution (Eb5 -> D5 -> C5)
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_DS5, 0.20), NOTE_SILENCE(0.10), // Eb5 (Same frequency as D#5)
    NOTE_SILENCE(0.15),                       // Rest
    NOTE(NOTE_D5, 0.10), NOTE_SILENCE(0.05),  // D5
    NOTE_SILENCE(0.30),                       // Rest
    NOTE(NOTE_C5, 0.20), NOTE_SILENCE(0.10)   // C5
};

// Size helper
#define MARIO_THEME_B_SIZE (sizeof(mario_theme_b) / sizeof(mario_theme_b[0]))

static music music_theme_b = {
    .notes = mario_theme_b,
    .size = MARIO_THEME_B_SIZE
};

static const note mario_theme_c[] = {
    // Measure 19 - The repetitive syncopated C5s
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.05),  // C5 (Eighth)
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.20),  // C5 (Delayed)
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.20),  // C5 (Delayed)
    NOTE_SILENCE(0.15),                       // Rest

    // Measure 20 - The walk up to resolve
    NOTE(NOTE_C5, 0.10), NOTE_SILENCE(0.05),  // C5 (Eighth)
    NOTE(NOTE_D5, 0.10), NOTE_SILENCE(0.20),  // D5
    NOTE(NOTE_E5, 0.20), NOTE_SILENCE(0.40)   // E5 (Quarter held out to resolve the loop)
};

// Size helper
#define MARIO_THEME_C_SIZE (sizeof(mario_theme_c) / sizeof(mario_theme_c[0]))

static music music_theme_c = {
    .notes = mario_theme_c,
    .size = MARIO_THEME_B_SIZE
};
