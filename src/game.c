#include "game.h"
#include "types.h"
#include <raylib.h>

int score = 0;
float songTime = 0.0f;
float keyTimers[NUM_COLUMNS] = {0};
bool keyHitVisual[NUM_COLUMNS] = {false};
float keyHitTimers[NUM_COLUMNS] = {0};
Note notes[MAX_NOTES];
int noteCount = 0;

int combo = 0;
int multiplier = 1;

static int comboThresholds[] = {5, 10, 20}; // x2 at 5, x3 at 10, x4 at 20

int GetCombo(void) { return combo; }
int GetMultiplier(void) { return multiplier; }

static void UpdateKeyTimers(float delta) {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    if (keyTimers[i] > 0)
      keyTimers[i] -= delta;
    if (keyHitVisual[i]) {
      keyHitTimers[i] -= delta;
      if (keyHitTimers[i] <= 0)
        keyHitVisual[i] = false;
    }
  }
}

static bool TryHitNote(int col) {
  for (int j = 0; j < noteCount; j++) {
    if (!notes[j].active || notes[j].column != col)
      continue;
    float dt = notes[j].time - songTime;
    if (dt < HIT_WINDOW / 300.0f && dt > -HIT_WINDOW / 300.0f) {
      notes[j].active = false;
      combo++;
      if (combo >= comboThresholds[2])
        multiplier = 4;
      else if (combo >= comboThresholds[1])
        multiplier = 3;
      else if (combo >= comboThresholds[0])
        multiplier = 2;
      else
        multiplier = 1;
      score += BASE_SCORE * multiplier;
      keyHitVisual[col] = true;
      keyHitTimers[col] = HIT_FEEDBACK_TIME;
      return true;
    }
  }
  return false;
}

static bool ProcessKeyPresses(void) {
  bool hit = false;
  for (int i = 0; i < NUM_COLUMNS; i++) {
    if (IsKeyPressed(BUTTON_KEYS[i]) && keyTimers[i] <= 0) {
      keyTimers[i] = KEY_DELAY;
      if (TryHitNote(i)) {
        hit = true;
      }
    }
  }
  return hit;
}

static void CheckMiss(bool hit) {
  if (!hit) {
    for (int i = 0; i < NUM_COLUMNS; i++) {
      if (IsKeyPressed(BUTTON_KEYS[i])) {
        combo = 0;
        multiplier = 1;
        break;
      }
    }
  }
}

void UpdateInput(float delta) {
  UpdateKeyTimers(delta);
  bool hit = ProcessKeyPresses();
  CheckMiss(hit);
}

void GameReset(void) {
  score = 0;
  combo = 0;
  multiplier = 1;
  songTime = 0.0f;
  for (int i = 0; i < NUM_COLUMNS; i++) {
    keyTimers[i] = 0.0f;
    keyHitVisual[i] = false;
    keyHitTimers[i] = 0.0f;
  }
  for (int i = 0; i < noteCount; i++) {
    notes[i].active = true;
  }
}
