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
void DrawResumeCountdown(int seconds);
void DrawMainMenu(bool *playPressed, bool *exitPressed);

#endif  // DRAW_H
