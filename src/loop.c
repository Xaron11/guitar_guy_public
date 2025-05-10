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
      DrawPauseOverlay();
    } else if (resumeCountdown > 0) {
      resumeTimer += GetFrameTime();
      if (resumeTimer >= 1.0f) {
        resumeCountdown--;
        resumeTimer = 0.0f;
      }
      if (resumeCountdown > 0) {
        DrawResumeCountdown(resumeCountdown);
      }
    }
    EndDrawing();
    if (resumeCountdown > 0) {
      continue;
    }
  }
}
