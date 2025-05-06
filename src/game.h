#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "types.h"

#define BASE_SCORE 100

void UpdateInput(float delta);
extern int score;
extern float songTime;
extern float keyTimers[NUM_COLUMNS];
extern bool keyHitVisual[NUM_COLUMNS];
extern float keyHitTimers[NUM_COLUMNS];
extern Note notes[MAX_NOTES];
extern int noteCount;

// Combo and multiplier
extern int combo;
extern int multiplier;
int GetCombo(void);
int GetMultiplier(void);

#endif  // GAME_H
