#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>

#include "types.h"

void DrawColumnLines(void);
void DrawButtons(const GameStateData *state, bool enableVisuals);
void DrawNotes(const GameStateData *state);
void DrawScoreUI(const GameStateData *state);
void DrawSongInfo(const GameStateData *state);
void DrawProgressBar(const GameStateData *state);
void DrawPauseOverlay(void);
void DrawPauseOverlayWithExit(bool *exitPressed);
void DrawResumeCountdown(int seconds);
void DrawMainMenu(bool *playPressed, bool *exitPressed);
void DrawLevelSelectMenu(const char **songNames, int songCount,
                         int *selectedIdx, bool *backPressed);

#endif  // DRAW_H
