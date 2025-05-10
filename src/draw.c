#include "draw.h"
#include "game.h"
#include "map.h"
#include "raygui.h"
#include "types.h"
#include <raylib.h>
#include <stdio.h>

extern float songTime;
extern float keyHitTimers[NUM_COLUMNS];
extern bool keyHitVisual[NUM_COLUMNS];
extern Note notes[MAX_NOTES];
extern int noteCount;

void DrawColumnLines() {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    int x = (i + 1) * SCREEN_WIDTH / (NUM_COLUMNS + 1);
    DrawLine(x, 0, x, SCREEN_HEIGHT, GRAY);
  }
}

void DrawButtons(bool enableVisuals) {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    float x = (float)((i + 1) * SCREEN_WIDTH) / (NUM_COLUMNS + 1);
    Color c = BUTTON_COLORS[i];
    if (enableVisuals) {
      if (IsKeyDown(BUTTON_KEYS[i]))
        c = Fade(c, 0.5f);
      if (keyHitVisual[i])
        c = WHITE;
    }
    Vector2 buttonPos = {x, (float)(SCREEN_HEIGHT - 50)};
    DrawRing(buttonPos, NOTE_RADIUS, NOTE_RADIUS + 5, 0, 360, 32, c);
  }
}

void DrawNotes() {
  for (int i = 0; i < noteCount; i++) {
    if (!notes[i].active)
      continue;
    float noteY =
        (float)SCREEN_HEIGHT - 50 - ((notes[i].time - songTime) * 300.0f);
    if (noteY > (float)SCREEN_HEIGHT || noteY < -NOTE_RADIUS)
      continue;
    float x = (float)((notes[i].column + 1) * SCREEN_WIDTH) / (NUM_COLUMNS + 1);
    DrawCircleLines((int)x, (int)noteY, NOTE_RADIUS,
                    BUTTON_COLORS[notes[i].column]);
  }
}

void DrawScoreUI() {
  DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 24) / 2, 40, 24,
           GRAY);
  DrawText(TextFormat("%d", score),
           GetScreenWidth() / 2 - MeasureText(TextFormat("%d", score), 64) / 2,
           70, 64, YELLOW);
  if (GetCombo() > 0) {
    DrawText(TextFormat("COMBO: %d", GetCombo()),
             GetScreenWidth() / 2 -
                 MeasureText(TextFormat("COMBO: %d", GetCombo()), 36) / 2,
             140, 36, ORANGE);
  }
  if (GetMultiplier() > 1) {
    DrawText(TextFormat("x%d", GetMultiplier()),
             GetScreenWidth() / 2 -
                 MeasureText(TextFormat("x%d", GetMultiplier()), 48) / 2,
             180, 48, RED);
  }
}

void DrawSongInfo() {
  DrawText(songTitle, 20, 40, 20, LIGHTGRAY);
  DrawText(songArtist, 20, 65, 18, DARKGRAY);
}

void DrawProgressBar() {
  int notesHit = 0;
  for (int i = 0; i < noteCount; i++) {
    if (!notes[i].active)
      notesHit++;
  }
  float progress =
      (noteCount > 0) ? ((float)notesHit / (float)noteCount) : 0.0f;
  Rectangle barBounds = {40.0f, 8.0f, (float)(GetScreenWidth() - 80), 24.0f};

  GuiProgressBar(barBounds, "", "", &progress, 0.0f, 1.0f);
}

void DrawPauseOverlay(void) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
  DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 64) / 2,
           GetScreenHeight() / 2 - 32, 64, YELLOW);
  DrawText("Press ESC to resume",
           GetScreenWidth() / 2 - MeasureText("Press ESC to resume", 24) / 2,
           GetScreenHeight() / 2 + 40, 24, GRAY);
}

void DrawResumeCountdown(int seconds) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", seconds);
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
  DrawText(buf, GetScreenWidth() / 2 - MeasureText(buf, 96) / 2,
           GetScreenHeight() / 2 - 48, 96, YELLOW);
}
