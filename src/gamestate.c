#include "gamestate.h"
#include "draw.h"
#include "game.h"
#include "loop.h"
#include "map.h"
#include "scores.h"
#include "types.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SONGS_DIR "assets/maps"

// Load only [Song] section from .chart file into SongEntry
void LoadSongMetadata(const char *filename, SongEntry *entry) {
  char *text = LoadFileText(filename);
  if (!text) {
    entry->title[0] = '\0';
    entry->artist[0] = '\0';
    entry->duration = 0.0f;
    return;
  }
  entry->title[0] = '\0';
  entry->artist[0] = '\0';
  entry->duration = 0.0f;
  int inSongSection = 0;
  char *saveptr = NULL;
  const char *line = strtok_r(text, "\n", &saveptr);
  while (line) {
    if (strstr(line, "[Song]")) {
      inSongSection = 1;
      line = strtok_r(NULL, "\n", &saveptr);
      continue;
    }
    if (inSongSection && line[0] == '[')
      break;
    if (inSongSection) {
      if (strstr(line, "Name")) {
        sscanf(line, "Name = \"%127[^\"]\"", entry->title);
      } else if (strstr(line, "Artist")) {
        sscanf(line, "Artist = \"%127[^\"]\"", entry->artist);
      }
    }
    if (inSongSection && strstr(line, "Notes")) {
      int notes = 0;
      if (sscanf(line, "Notes = %d", &notes) == 1) {
        entry->duration = (float)notes; // Placeholder: can be improved
      }
    }
    line = strtok_r(NULL, "\n", &saveptr);
  }
  UnloadFileText(text);
}

void ScanAvailableSongs(GameContext *ctx) {
  if (ctx->songList.entries) {
    free(ctx->songList.entries);
    ctx->songList.entries = NULL;
    ctx->songList.count = 0;
  }
  FilePathList files = LoadDirectoryFilesEx(SONGS_DIR, ".chart", false);
  size_t count = 0;
  for (unsigned int i = 0; i < files.count; i++) {
    if (IsFileExtension(files.paths[i], ".chart"))
      count++;
  }
  if (count == 0) {
    ctx->songList.entries = NULL;
    ctx->songList.count = 0;
    UnloadDirectoryFiles(files);
    return;
  }
  SongEntry *entries = (SongEntry *)calloc(count, sizeof(SongEntry));
  ctx->songList.entries = entries;
  ctx->songList.count = count;
  size_t idx = 0;
  for (unsigned int i = 0; i < files.count && idx < count; i++) {
    if (IsFileExtension(files.paths[i], ".chart")) {
      SongEntry *entry = &entries[idx];
      snprintf(entry->path, sizeof(entry->path), "%s", files.paths[i]);
      LoadSongMetadata(files.paths[i], entry);
      idx++;
    }
  }
  UnloadDirectoryFiles(files);
}

const SongList *GetSongList(const GameContext *ctx) { return &ctx->songList; }

void SetCurrentSong(GameContext *ctx, int songIdx) {
  ctx->currentSongIdx = songIdx;
  ctx->songLoaded = false;
}

GameState HandleMenuState(const GameContext *ctx, bool *shouldExit) {
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

GameState HandleLevelSelectState(const GameContext *ctx, bool *backToMenu,
                                 int *selectedSongIdx) {
  static int selectedIdx = -1;
  *backToMenu = false;
  *selectedSongIdx = -1;
  const SongList *list = GetSongList(ctx);
  if (!list || !list->entries || list->count <= 0) {
    return STATE_MENU;
  }
  static char buttonLabels[128][256];
  const char *labelPtrs[128];
  int shownCount = 0;
  for (int i = 0; i < list->count && i < 128; i++) {
    const char *fileName = GetFileNameWithoutExt(list->entries[i].path);
    snprintf(buttonLabels[shownCount], sizeof(buttonLabels[shownCount]),
             "%s (%s)", fileName,
             list->entries[i].title[0] ? list->entries[i].title : "Unknown");
    labelPtrs[shownCount] = buttonLabels[shownCount];
    shownCount++;
  }
  if (shownCount <= 0) {
    return STATE_MENU;
  }
  bool backPressed = false;
  DrawLevelSelectMenu(ctx, labelPtrs, shownCount, &selectedIdx, &backPressed);
  if (selectedIdx >= 0 && selectedIdx < list->count) {
    *selectedSongIdx = selectedIdx;
    selectedIdx = -1;
    return STATE_PLAYING;
  } else if (backPressed) {
    *backToMenu = true;
    return STATE_MENU;
  }
  return STATE_LEVEL_SELECT;
}

GameState HandlePlayingState(GameContext *ctx, int *currentHighscore) {
  if (!ctx->songLoaded && ctx->currentSongIdx >= 0 &&
      ctx->currentSongIdx < ctx->songList.count) {
    LoadMap(ctx->songList.entries[ctx->currentSongIdx].path,
            &ctx->gameState.currentSong);
    GameReset(&ctx->gameState);
    ctx->songLoaded = true;
  }
  if (IsKeyPressed(KEY_ESCAPE)) {
    return STATE_PAUSED;
  }
  float delta = GetFrameTime();
  ctx->gameState.songTime += delta;
  UpdateInput(&ctx->gameState, delta);
  DrawColumnLines();
  DrawScoreUI(&ctx->gameState);
  DrawSongInfo(&ctx->gameState);
  DrawProgressBar(&ctx->gameState);
  DrawButtons(&ctx->gameState, true);
  DrawNotes(&ctx->gameState);
  if (IsSongFinished(&ctx->gameState)) {
    *currentHighscore = GetHighscore(
        &ctx->highscores, ctx->songList.entries[ctx->currentSongIdx].path);
    if (ctx->gameState.score > *currentHighscore) {
      *currentHighscore = ctx->gameState.score;
      SetHighscore(&ctx->highscores,
                   ctx->songList.entries[ctx->currentSongIdx].path,
                   ctx->gameState.score);
      SaveHighscores(&ctx->highscores, "assets/scores.csv");
    }
    ctx->songLoaded = false;
    return STATE_RESULTS;
  }
  return STATE_PLAYING;
}

GameState HandlePausedState(const GameContext *ctx, int *resumeCountdown,
                            float *resumeTimer) {
  DrawColumnLines();
  DrawScoreUI(&ctx->gameState);
  DrawSongInfo(&ctx->gameState);
  DrawProgressBar(&ctx->gameState);
  DrawButtons(&ctx->gameState, false);
  DrawNotes(&ctx->gameState);
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

GameState HandleResumeState(const GameContext *ctx, int *resumeCountdown,
                            float *resumeTimer) {
  DrawColumnLines();
  DrawScoreUI(&ctx->gameState);
  DrawSongInfo(&ctx->gameState);
  DrawProgressBar(&ctx->gameState);
  DrawButtons(&ctx->gameState, false);
  DrawNotes(&ctx->gameState);
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

GameState HandleResultsState(GameContext *ctx, int currentHighscore) {
  bool menuPressed = false;

  DrawResultsScreen(&ctx->gameState, &menuPressed, currentHighscore);
  if (menuPressed) {
    ctx->songLoaded = false;
    return STATE_MENU;
  }
  return STATE_RESULTS;
}