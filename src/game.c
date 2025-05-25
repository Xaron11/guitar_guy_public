#include "game.h"
#include "types.h"
#include <raylib.h>

#define BASE_SCORE 100
#define DEFAULT_SONG_OFFSET 3.0f
static int comboThresholds[] = {5, 10, 20}; // x2 at 5, x3 at 10, x4 at 20

void CalculateSongDuration(Song *song) {
  float maxTime = 0.0f;
  for (int i = 0; i < song->noteCount; i++) {
    if (song->notes[i].time > maxTime)
      maxTime = song->notes[i].time;
  }
  song->duration = maxTime + 1.0f;
}

void GameReset(GameStateData *state) {
  state->score = 0;
  state->combo = 0;
  state->multiplier = 1;
  state->songTime = -(state->currentSong.offset + DEFAULT_SONG_OFFSET);
  for (int i = 0; i < NUM_COLUMNS; i++) {
    state->keyHitVisual[i] = false;
    state->keyHitTimers[i] = 0.0f;
    state->keyTimers[i] = 0.0f;
  }
  for (int i = 0; i < state->currentSong.noteCount; i++) {
    state->currentSong.notes[i].active = true;
  }
}

float GetSongProgress(const GameStateData *state) {
  if (state->currentSong.duration <= 0.0f)
    return 0.0f;
  float progress = state->songTime / state->currentSong.duration;
  if (progress > 1.0f)
    progress = 1.0f;
  return progress;
}

bool IsSongFinished(const GameStateData *state) {
  return state->songTime >= state->currentSong.duration;
}

static void UpdateKeyTimers(GameStateData *state, float delta) {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    if (state->keyTimers[i] > 0)
      state->keyTimers[i] -= delta;
    if (state->keyHitVisual[i]) {
      state->keyHitTimers[i] -= delta;
      if (state->keyHitTimers[i] <= 0)
        state->keyHitVisual[i] = false;
    }
  }
}

static bool TryHitNote(GameStateData *state, int col) {
  for (int j = 0; j < state->currentSong.noteCount; j++) {
    if (!state->currentSong.notes[j].active ||
        state->currentSong.notes[j].column != col)
      continue;
    float dt = state->currentSong.notes[j].time - state->songTime;
    if (dt < HIT_WINDOW / 300.0f && dt > -HIT_WINDOW / 300.0f) {
      state->currentSong.notes[j].active = false;
      state->combo++;
      if (state->combo >= comboThresholds[2])
        state->multiplier = 4;
      else if (state->combo >= comboThresholds[1])
        state->multiplier = 3;
      else if (state->combo >= comboThresholds[0])
        state->multiplier = 2;
      else
        state->multiplier = 1;
      state->score += BASE_SCORE * state->multiplier;
      state->keyHitVisual[col] = true;
      state->keyHitTimers[col] = HIT_FEEDBACK_TIME;
      return true;
    }
  }
  return false;
}

static bool ProcessKeyPresses(GameStateData *state) {
  bool hit = false;
  for (int i = 0; i < NUM_COLUMNS; i++) {
    if (IsKeyPressed(BUTTON_KEYS[i]) && state->keyTimers[i] <= 0) {
      state->keyTimers[i] = KEY_DELAY;
      if (TryHitNote(state, i)) {
        hit = true;
      }
    }
  }
  return hit;
}

static void CheckMiss(GameStateData *state, bool hit) {
  if (!hit) {
    for (int i = 0; i < NUM_COLUMNS; i++) {
      if (IsKeyPressed(BUTTON_KEYS[i])) {
        state->combo = 0;
        state->multiplier = 1;
        break;
      }
    }
  }
}

void UpdateInput(GameStateData *state, float delta) {
  UpdateKeyTimers(state, delta);
  bool hit = ProcessKeyPresses(state);
  CheckMiss(state, hit);
}
