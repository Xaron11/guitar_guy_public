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
  float progress = GetSongProgress();
  Rectangle barBounds = {40.0f, 8.0f, (float)(GetScreenWidth() - 80), 24.0f};

  GuiProgressBar(barBounds, "", "", &progress, 0.0f, 1.0f);
}

void DrawPauseOverlay(void) {
  bool dummy = false;
  DrawPauseOverlayWithExit(&dummy);
}

void DrawPauseOverlayWithExit(bool *exitPressed) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
  DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 64) / 2,
           GetScreenHeight() / 2 - 32, 64, YELLOW);
  DrawText("Press ESC to resume",
           GetScreenWidth() / 2 - MeasureText("Press ESC to resume", 24) / 2,
           GetScreenHeight() / 2 + 40, 24, GRAY);
  *exitPressed = GuiButton((Rectangle){10, GetScreenHeight() - 60, 200, 50},
                           "EXIT TO MENU");
}

void DrawResumeCountdown(int seconds) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", seconds);
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
  DrawText(buf, GetScreenWidth() / 2 - MeasureText(buf, 96) / 2,
           GetScreenHeight() / 2 - 48, 96, YELLOW);
}

void DrawMainMenu(bool *playPressed, bool *exitPressed) {
  const char *title = "C-GAME";
  int titleFontSize = 64;
  int btnWidth = 200;
  int btnHeight = 60;
  int btnX = GetScreenWidth() / 2 - btnWidth / 2;
  int btnY = GetScreenHeight() / 2 - btnHeight / 2 + 60;
  int exitBtnY = btnY + btnHeight + 30;
  DrawText(title, GetScreenWidth() / 2 - MeasureText(title, titleFontSize) / 2,
           100, titleFontSize, YELLOW);
  DrawText("A simple rhythm game",
           GetScreenWidth() / 2 - MeasureText("A simple rhythm game", 24) / 2,
           180, 24, GRAY);

  int prevStyle = GuiGetStyle(DEFAULT, TEXT_SIZE);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
  *playPressed = GuiButton(
      (Rectangle){(float)btnX, (float)btnY, (float)btnWidth, (float)btnHeight},
      "PLAY");
  *exitPressed = GuiButton((Rectangle){(float)btnX, (float)exitBtnY,
                                       (float)btnWidth, (float)btnHeight},
                           "EXIT");
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevStyle);
}

void DrawLevelSelectMenu(bool *song1Pressed, bool *song2Pressed,
                         bool *backPressed) {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  int btnWidth = 320;
  int btnHeight = 70;
  int spacing = 40;
  int yStart = screenHeight / 2 - btnHeight - spacing / 2;
  int x = screenWidth / 2 - btnWidth / 2;

  DrawText("Select Level",
           screenWidth / 2 - MeasureText("Select Level", 48) / 2, 100, 48,
           YELLOW);

  *song1Pressed = GuiButton(
      (Rectangle){(float)x, (float)yStart, (float)btnWidth, (float)btnHeight},
      "Song 1");
  *song2Pressed =
      GuiButton((Rectangle){(float)x, (float)(yStart + btnHeight + spacing),
                            (float)btnWidth, (float)btnHeight},
                "Song 2");
  *backPressed = GuiButton(
      (Rectangle){40.0f, (float)screenHeight - 80.0f, 180.0f, 50.0f}, "BACK");
}
