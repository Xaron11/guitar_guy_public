#include "gamestate.h"
#include "draw.h"
#include "game.h"
#include "loop.h"
#include <raylib.h>

GameState HandleMenuState(bool *shouldExit) {
  bool playPressed = false;
  bool exitPressed = false;
  DrawMainMenu(&playPressed, &exitPressed);
  if (playPressed) {
    GameReset();
    return STATE_PLAYING;
  } else if (exitPressed) {
    *shouldExit = true;
  }
  return STATE_MENU;
}

GameState HandlePlayingState(void) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    return STATE_PAUSED;
  }
  float delta = GetFrameTime();
  songTime += delta;
  UpdateInput(delta);
  DrawColumnLines();
  DrawScoreUI();
  DrawSongInfo();
  DrawProgressBar();
  DrawButtons(true);
  DrawNotes();
  if (IsSongFinished()) {
    return STATE_MENU;
  }
  return STATE_PLAYING;
}

GameState HandlePausedState(int *resumeCountdown, float *resumeTimer) {
  DrawColumnLines();
  DrawScoreUI();
  DrawSongInfo();
  DrawProgressBar();
  DrawButtons(false);
  DrawNotes();
  DrawPauseOverlay();
  if (IsKeyPressed(KEY_ESCAPE)) {
    *resumeCountdown = 3;
    *resumeTimer = 0.0f;
    return STATE_RESUME;
  }
  return STATE_PAUSED;
}

GameState HandleResumeState(int *resumeCountdown, float *resumeTimer) {
  DrawColumnLines();
  DrawScoreUI();
  DrawSongInfo();
  DrawProgressBar();
  DrawButtons(false);
  DrawNotes();
  *resumeTimer += GetFrameTime();
  if (*resumeTimer >= 1.0f) {
    (*resumeCountdown)--;
    *resumeTimer = 0.0f;
  }
  if (*resumeCountdown > 0) {
    DrawResumeCountdown(*resumeCountdown);
    return STATE_RESUME;
  } else {
    return STATE_PLAYING;
  }
}
