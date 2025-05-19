#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>

#include "loop.h"

GameState HandleMenuState(bool *shouldExit);
GameState HandlePlayingState(void);
GameState HandlePausedState(int *resumeCountdown, float *resumeTimer);
GameState HandleResumeState(int *resumeCountdown, float *resumeTimer);

#endif  // GAMESTATE_H
