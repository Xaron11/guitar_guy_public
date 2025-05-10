#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "draw.h"
#include "game.h"
#include "loop.h"
#include <raylib.h>

extern int score;
extern float songTime;
extern char songTitle[128];
extern char songArtist[128];

void GameLoop(void) {
  typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_RESUME
  } GameState;
  GameState state = STATE_MENU;
  int resumeCountdown = 0;
  float resumeTimer = 0.0f;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    if (state == STATE_MENU) {
      const char *title = "C-GAME";
      int titleFontSize = 64;
      int btnWidth = 200;
      int btnHeight = 60;
      int btnX = GetScreenWidth() / 2 - btnWidth / 2;
      int btnY = GetScreenHeight() / 2 - btnHeight / 2 + 60;
      DrawText(title,
               GetScreenWidth() / 2 - MeasureText(title, titleFontSize) / 2,
               100, titleFontSize, YELLOW);
      DrawText("A simple rhythm game",
               GetScreenWidth() / 2 -
                   MeasureText("A simple rhythm game", 24) / 2,
               180, 24, GRAY);
      bool playPressed =
          GuiButton((Rectangle){btnX, btnY, btnWidth, btnHeight}, "Play");
      if (playPressed) {
        GameReset();
        state = STATE_PLAYING;
      }
    } else if (state == STATE_PLAYING || state == STATE_PAUSED ||
               state == STATE_RESUME) {
      if (state == STATE_PLAYING && IsKeyPressed(KEY_ESCAPE)) {
        state = STATE_PAUSED;
      } else if (state == STATE_PAUSED && IsKeyPressed(KEY_ESCAPE)) {
        resumeCountdown = 3;
        resumeTimer = 0.0f;
        state = STATE_RESUME;
      }
      if (state == STATE_PLAYING && resumeCountdown == 0) {
        float delta = GetFrameTime();
        songTime += delta;
        UpdateInput(delta);
      }
      DrawColumnLines();
      DrawScoreUI();
      DrawSongInfo();
      DrawProgressBar();
      DrawButtons(state == STATE_PLAYING && resumeCountdown == 0);
      DrawNotes();
      if (state == STATE_PAUSED) {
        DrawPauseOverlay();
      } else if (state == STATE_RESUME) {
        resumeTimer += GetFrameTime();
        if (resumeTimer >= 1.0f) {
          resumeCountdown--;
          resumeTimer = 0.0f;
        }
        if (resumeCountdown > 0) {
          DrawResumeCountdown(resumeCountdown);
        } else {
          state = STATE_PLAYING;
        }
      }
    }
    EndDrawing();
    if (state == STATE_RESUME && resumeCountdown > 0) {
      continue;
    }
  }
}
