#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "types.h"

void UpdateInput(float delta);
extern int score;
extern float songTime;
extern float keyTimers[NUM_COLUMNS];
extern bool keyHitVisual[NUM_COLUMNS];
extern float keyHitTimers[NUM_COLUMNS];
extern Note notes[MAX_NOTES];
extern int noteCount;

#endif  // GAME_H
