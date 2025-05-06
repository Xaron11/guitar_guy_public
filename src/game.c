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

void UpdateInput(float delta) {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    if (keyTimers[i] > 0)
      keyTimers[i] -= delta;
    if (keyHitVisual[i]) {
      keyHitTimers[i] -= delta;
      if (keyHitTimers[i] <= 0)
        keyHitVisual[i] = false;
    }
    if (IsKeyPressed(BUTTON_KEYS[i]) && keyTimers[i] <= 0) {
      keyTimers[i] = KEY_DELAY;
      for (int j = 0; j < noteCount; j++) {
        if (!notes[j].active || notes[j].column != i)
          continue;
        float dt = notes[j].time - songTime;
        if (dt < HIT_WINDOW / 300.0f && dt > -HIT_WINDOW / 300.0f) {
          notes[j].active = false;
          score += 100;
          keyHitVisual[i] = true;
          keyHitTimers[i] = HIT_FEEDBACK_TIME;
          break;
        }
      }
    }
  }
}
