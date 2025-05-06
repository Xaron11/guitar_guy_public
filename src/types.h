#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

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

#endif  // TYPES_H
