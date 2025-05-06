#include "draw.h"
#include "types.h"
#include <raylib.h>

extern float songTime;
extern float keyHitTimers[NUM_COLUMNS];
extern bool keyHitVisual[NUM_COLUMNS];
extern Note notes[MAX_NOTES];
extern int noteCount;

void DrawColumnLines() {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    int x = (i + 1) * SCREEN_WIDTH / (NUM_COLUMNS + 1);
    DrawLine(x, 0, x, SCREEN_HEIGHT, GRAY);
  }
}

void DrawButtons() {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    float x = (float)((i + 1) * SCREEN_WIDTH) / (NUM_COLUMNS + 1);
    Color c = BUTTON_COLORS[i];
    if (IsKeyDown(BUTTON_KEYS[i]))
      c = Fade(c, 0.5f);
    if (keyHitVisual[i])
      c = WHITE;
    Vector2 buttonPos = {x, (float)(SCREEN_HEIGHT - 50)};
    DrawRing(buttonPos, NOTE_RADIUS, NOTE_RADIUS + 5, 0, 360, 32, c);
  }
}

void DrawNotes() {
  for (int i = 0; i < noteCount; i++) {
    if (!notes[i].active)
      continue;
    float noteY =
        (float)SCREEN_HEIGHT - 50 - ((notes[i].time - songTime) * 300.0f);
    if (noteY > (float)SCREEN_HEIGHT || noteY < -NOTE_RADIUS)
      continue;
    float x = (float)((notes[i].column + 1) * SCREEN_WIDTH) / (NUM_COLUMNS + 1);
    DrawCircleLines((int)x, (int)noteY, NOTE_RADIUS,
                    BUTTON_COLORS[notes[i].column]);
  }
}
