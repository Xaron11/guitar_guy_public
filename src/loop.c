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
  char *selectedSong = NULL;
  bool backToMenu = false;

  while (!WindowShouldClose() && !shouldExit) {
    BeginDrawing();
    ClearBackground(BLACK);
    switch (state) {
    case STATE_MENU:
      state = HandleMenuState(&shouldExit);
      selectedSong = NULL;
      break;
    case STATE_LEVEL_SELECT:
      state = HandleLevelSelectState(&backToMenu, &selectedSong);
      if (backToMenu) {
        state = STATE_MENU;
        backToMenu = false;
      }
      break;
    case STATE_PLAYING:
      if (selectedSong) {
        SetCurrentSong(selectedSong);
        selectedSong = NULL;
      }
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
