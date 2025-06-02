#include "scores.h"
#define RAYGUI_IMPLEMENTATION
#include "gamestate.h"
#include "loop.h"
#include "raygui.h"
#include <raylib.h>

void GameLoop(void) {
  GameContext ctx = {0};
  ScanAvailableSongs(&ctx);
  LoadHighscores(&ctx.highscores, "assets/scores.csv");
  GameState state = STATE_MENU;
  int resumeCountdown = 0;
  float resumeTimer = 0.0f;
  bool shouldExit = false;
  int selectedSongIdx = -1;
  bool backToMenu = false;
  int currentHighscore = 0;

  while (!WindowShouldClose() && !shouldExit) {
    BeginDrawing();
    ClearBackground(BLACK);
    switch (state) {
    case STATE_MENU:
      state = HandleMenuState(&ctx, &shouldExit);
      selectedSongIdx = -1;
      break;
    case STATE_LEVEL_SELECT:
      state = HandleLevelSelectState(&ctx, &backToMenu, &selectedSongIdx);
      if (backToMenu) {
        state = STATE_MENU;
        backToMenu = false;
      }
      break;
    case STATE_PLAYING:
      if (selectedSongIdx >= 0) {
        SetCurrentSong(&ctx, selectedSongIdx);
        selectedSongIdx = -1;
      }
      state = HandlePlayingState(&ctx, &currentHighscore);
      break;
    case STATE_PAUSED:
      state = HandlePausedState(&ctx, &resumeCountdown, &resumeTimer);
      break;
    case STATE_RESUME:
      state = HandleResumeState(&ctx, &resumeCountdown, &resumeTimer);
      break;
    case STATE_RESULTS:
      state = HandleResultsState(&ctx, currentHighscore);
      break;
    }
    EndDrawing();
  }
}
