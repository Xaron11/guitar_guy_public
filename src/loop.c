#define RAYGUI_IMPLEMENTATION
#include "loop.h"
#include "gamestate.h"
#include "raygui.h"
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
    switch (state) {
    case STATE_MENU:
      state = HandleMenuState(&shouldExit);
      break;
    case STATE_PLAYING:
      state = HandlePlayingState();
      break;
    case STATE_PAUSED:
      state = HandlePausedState(&resumeCountdown, &resumeTimer);
      break;
    case STATE_RESUME:
      state = HandleResumeState(&resumeCountdown, &resumeTimer);
      break;
    }
    EndDrawing();
  }
}
