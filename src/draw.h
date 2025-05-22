#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>

void DrawColumnLines(void);
void DrawButtons(bool enableVisuals);
void DrawNotes(void);
void DrawScoreUI(void);
void DrawSongInfo(void);
void DrawProgressBar(void);
void DrawPauseOverlay(void);
void DrawPauseOverlayWithExit(bool *exitPressed);
void DrawResumeCountdown(int seconds);
void DrawMainMenu(bool *playPressed, bool *exitPressed);
void DrawLevelSelectMenu(bool *song1Pressed, bool *song2Pressed,
                         bool *backPressed);

#endif  // DRAW_H
