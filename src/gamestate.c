#include "gamestate.h"
#include "draw.h"
#include "game.h"
#include "loop.h"
#include "map.h"
#include <raylib.h>
#include <stddef.h>

static const char *currentSong = NULL;
static bool songLoaded = false;

void SetCurrentSong(const char *songPath) {
  currentSong = songPath;
  songLoaded = false;
}

GameState HandleMenuState(bool *shouldExit) {
  bool playPressed = false;
  bool exitPressed = false;
  DrawMainMenu(&playPressed, &exitPressed);
  if (playPressed) {
    return STATE_LEVEL_SELECT;
  } else if (exitPressed) {
    *shouldExit = true;
  }
  return STATE_MENU;
}

GameState HandleLevelSelectState(bool *backToMenu, char **selectedSong) {
  bool song1Pressed = false;
  bool song2Pressed = false;
  bool backPressed = false;
  DrawLevelSelectMenu(&song1Pressed, &song2Pressed, &backPressed);
  if (song1Pressed) {
    *selectedSong = "assets/song1.chart";
    return STATE_PLAYING;
  } else if (song2Pressed) {
    *selectedSong = "assets/song2.chart";
    return STATE_PLAYING;
  } else if (backPressed) {
    *backToMenu = true;
    return STATE_MENU;
  }
  return STATE_LEVEL_SELECT;
}

GameState HandlePlayingState(void) {
  if (!songLoaded && currentSong) {
    LoadMap(currentSong);
    CalculateSongDuration();
    GameReset();
    songLoaded = true;
  }
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
    songLoaded = false;
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
  bool exitPressed = false;
  DrawPauseOverlayWithExit(&exitPressed);
  if (exitPressed) {
    return STATE_MENU;
  }
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
