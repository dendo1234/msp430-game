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

static const note mario_notes[] = {
    // ==========================================
    // INTRO (14 macros)
    // ==========================================
    NOTE(659, 0.10), NOTE_SILENCE(0.05),  // E5
    NOTE(659, 0.10), NOTE_SILENCE(0.20),  // E5
    NOTE(659, 0.10), NOTE_SILENCE(0.20),  // E5
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(659, 0.20), NOTE_SILENCE(0.10),  // E5
    NOTE(784, 0.20), NOTE_SILENCE(0.40),  // G5
    NOTE(392, 0.20), NOTE_SILENCE(0.40),  // G4

    // ==========================================
    // THEME A - First Pass (34 macros)
    // ==========================================
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

    // ==========================================
    // THEME A - Second Pass (34 macros)
    // ==========================================
    NOTE(523, 0.20), NOTE_SILENCE(0.25),  // C5
    NOTE(392, 0.10), NOTE_SILENCE(0.20),  // G4
    NOTE(330, 0.20), NOTE_SILENCE(0.25),  // E4
    NOTE(440, 0.20), NOTE_SILENCE(0.10),  // A4
    NOTE(494, 0.20), NOTE_SILENCE(0.10),  // B4
    NOTE(466, 0.10), NOTE_SILENCE(0.05),  // Bb4
    NOTE(440, 0.20), NOTE_SILENCE(0.10),  // A4
    NOTE(392, 0.10), NOTE_SILENCE(0.05),  // G4
    NOTE(659, 0.10), NOTE_SILENCE(0.05),  // E5
    NOTE(784, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(880, 0.20), NOTE_SILENCE(0.10),  // A5
    NOTE(698, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(784, 0.20), NOTE_SILENCE(0.10),  // G5
    NOTE(659, 0.20), NOTE_SILENCE(0.10),  // E5
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(587, 0.10), NOTE_SILENCE(0.05),  // D5
    NOTE(494, 0.20), NOTE_SILENCE(0.25),  // B4

    // ==========================================
    // THEME B - Bridge Part 1 (24 macros)
    // ==========================================
    NOTE_SILENCE(0.15),                   // Rest
    NOTE(784, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(740, 0.10), NOTE_SILENCE(0.05),  // F#5
    NOTE(698, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(622, 0.20), NOTE_SILENCE(0.10),  // D#5
    NOTE(659, 0.10), NOTE_SILENCE(0.20),  // E5
    NOTE(415, 0.10), NOTE_SILENCE(0.05),  // G#4
    NOTE(440, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE_SILENCE(0.15),                   // Rest
    NOTE(440, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(587, 0.20), NOTE_SILENCE(0.25),  // D5

    // ==========================================
    // THEME B - Bridge Part 2 (17 macros)
    // ==========================================
    NOTE_SILENCE(0.15),                   // Rest
    NOTE(784, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(740, 0.10), NOTE_SILENCE(0.05),  // F#5
    NOTE(698, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(622, 0.20), NOTE_SILENCE(0.10),  // D#5
    NOTE(659, 0.10), NOTE_SILENCE(0.20),  // E5
    NOTE(1047, 0.20), NOTE_SILENCE(0.10), // C6
    NOTE(1047, 0.10), NOTE_SILENCE(0.05), // C6
    NOTE(1047, 0.20), NOTE_SILENCE(0.40), // C6

    // ==========================================
    // THEME B - Bridge Part 3 (24 macros)
    // ==========================================
    NOTE_SILENCE(0.15),                   // Rest
    NOTE(784, 0.10), NOTE_SILENCE(0.05),  // G5
    NOTE(740, 0.10), NOTE_SILENCE(0.05),  // F#5
    NOTE(698, 0.10), NOTE_SILENCE(0.05),  // F5
    NOTE(622, 0.20), NOTE_SILENCE(0.10),  // D#5
    NOTE(659, 0.10), NOTE_SILENCE(0.20),  // E5
    NOTE(415, 0.10), NOTE_SILENCE(0.05),  // G#4
    NOTE(440, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE_SILENCE(0.15),                   // Rest
    NOTE(440, 0.10), NOTE_SILENCE(0.05),  // A4
    NOTE(523, 0.10), NOTE_SILENCE(0.05),  // C5
    NOTE(587, 0.20), NOTE_SILENCE(0.25),  // D5

    // ==========================================
    // THEME B - Bridge Ending (7 macros)
    // ==========================================
    NOTE_SILENCE(0.15),                   // Rest
    NOTE(622, 0.20), NOTE_SILENCE(0.25),  // D#5
    NOTE(587, 0.20), NOTE_SILENCE(0.25),  // D5
    NOTE(523, 0.20), NOTE_SILENCE(0.85)   // C5 (Longer rest before looping back)
};

static music mario_theme = {
    .notes = mario_notes,
    .size = 154
};


static const note teste_notes[] = {
    NOTE(E5, 0.10), NOTE_SILENCE(0.05),
    NOTE(E5, 0.10), NOTE_SILENCE(0.20),
    NOTE(E5, 0.10), NOTE_SILENCE(0.05),
    NOTE(E5, 0.10), NOTE_SILENCE(0.20),
};

static music teste_theme = {
    .notes = teste_notes,
    .size = 8
};

static const note new_mario[] = {
    NOTE(E5, 0.10), NOTE_SILENCE(0.05),
    NOTE(E5, 0.10), NOTE_SILENCE(0.20),
    NOTE(E5, 0.10), NOTE_SILENCE(0.05),
    NOTE(E5, 0.10), NOTE_SILENCE(0.20),
};
