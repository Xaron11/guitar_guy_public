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

  GameState state = STATE_MENU;
  int resumeCountdown = 0;
  float resumeTimer = 0.0f;
  bool shouldExit = false;

  while (!WindowShouldClose() && !shouldExit) {
    BeginDrawing();
    ClearBackground(BLACK);

    if (state == STATE_MENU) {
      bool playPressed = false;
      bool exitPressed = false;
      DrawMainMenu(&playPressed, &exitPressed);
      if (playPressed) {
        GameReset();
        state = STATE_PLAYING;
      } else if (exitPressed) {
        shouldExit = true;
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
      // Return to menu after song is finished
      if (state == STATE_PLAYING && IsSongFinished()) {
        state = STATE_MENU;
      }
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
