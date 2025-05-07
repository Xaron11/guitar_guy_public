#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "draw.h"
#include "game.h"
#include "loop.h"
#include "map.h"
#include <raylib.h>

extern int score;
extern float songTime;
extern char songTitle[128];
extern char songArtist[128];

void GameLoop(void) {
  bool paused = false;
  int resumeCountdown = 0;
  float resumeTimer = 0.0f;
  while (!WindowShouldClose()) {
    if (!paused && IsKeyPressed(KEY_ESCAPE)) {
      paused = true;
    } else if (paused && IsKeyPressed(KEY_ESCAPE)) {
      resumeCountdown = 3; // seconds
      resumeTimer = 0.0f;
      paused = false;
    }
    if (!paused && resumeCountdown == 0) {
      float delta = GetFrameTime();
      songTime += delta;
      UpdateInput(delta);
    }
    BeginDrawing();
    ClearBackground(BLACK);
    DrawColumnLines();
    DrawScoreUI();
    DrawSongInfo();
    DrawProgressBar();
    DrawButtons(!paused && resumeCountdown == 0);
    DrawNotes();
    if (paused) {
      DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                    Fade(BLACK, 0.6f));
      DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 64) / 2,
               GetScreenHeight() / 2 - 32, 64, YELLOW);
      DrawText("Press ESC to resume",
               GetScreenWidth() / 2 -
                   MeasureText("Press ESC to resume", 24) / 2,
               GetScreenHeight() / 2 + 40, 24, GRAY);
    } else if (resumeCountdown > 0) {
      resumeTimer += GetFrameTime();
      if (resumeTimer >= 1.0f) {
        resumeCountdown--;
        resumeTimer = 0.0f;
      }
      if (resumeCountdown > 0) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", resumeCountdown);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                      Fade(BLACK, 0.5f));
        DrawText(buf, GetScreenWidth() / 2 - MeasureText(buf, 96) / 2,
                 GetScreenHeight() / 2 - 48, 96, YELLOW);
      }
    }
    EndDrawing();
    if (resumeCountdown > 0) {
      continue;
    }
  }
}
