#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stddef.h>

#include "raylib.h"

#define NUM_COLUMNS 4
#define NOTE_RADIUS 20
#define HIT_WINDOW 30
#define MAX_NOTES 2048
#define HIT_FEEDBACK_TIME 0.2f
#define KEY_DELAY 0.2f

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const Color BUTTON_COLORS[NUM_COLUMNS];
extern const int BUTTON_KEYS[NUM_COLUMNS];

typedef struct Note {
  int column;
  float time;
  bool active;
} Note;

// Song metadata and note data
typedef struct {
  char title[128];
  char artist[128];
  int resolution;
  int tempo;
  float offset;
  int difficulty;
  Note notes[MAX_NOTES];
  int noteCount;
  float duration;  // in seconds
} Song;

// Metadata for song selection
typedef struct {
  char path[256];
  char title[128];
  char artist[128];
  float duration;  // in seconds
} SongEntry;

// List of available songs
typedef struct {
  SongEntry *entries;
  size_t count;
} SongList;

// State for the current game session
typedef struct {
  int score;
  float songTime;
  int combo;
  int maxCombo;
  int multiplier;
  float keyTimers[NUM_COLUMNS];
  bool keyHitVisual[NUM_COLUMNS];
  float keyHitTimers[NUM_COLUMNS];
  Song currentSong;
} GameStateData;

// Global game context
typedef struct {
  SongList songList;
  GameStateData gameState;
  int currentSongIdx;
  bool songLoaded;
} GameContext;

#endif  // TYPES_H
