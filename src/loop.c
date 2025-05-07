#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "draw.h"
#include "game.h"
#include "loop.h"
#include "map.h"
#include <raylib.h>

extern int score;
extern float songTime;
extern char songTitle[128];
extern char songArtist[128];

void GameLoop(void) {
  while (!WindowShouldClose()) {
    float delta = GetFrameTime();
    songTime += delta;
    UpdateInput(delta);

    BeginDrawing();
    ClearBackground(BLACK);

    DrawColumnLines();
    DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 24) / 2, 40,
             24, GRAY);
    DrawText(TextFormat("%d", score),
             GetScreenWidth() / 2 -
                 MeasureText(TextFormat("%d", score), 64) / 2,
             70, 64, YELLOW);

    if (GetCombo() > 0) {
      DrawText(TextFormat("COMBO: %d", GetCombo()),
               GetScreenWidth() / 2 -
                   MeasureText(TextFormat("COMBO: %d", GetCombo()), 36) / 2,
               140, 36, ORANGE);
    }
    if (GetMultiplier() > 1) {
      DrawText(TextFormat("x%d", GetMultiplier()),
               GetScreenWidth() / 2 -
                   MeasureText(TextFormat("x%d", GetMultiplier()), 48) / 2,
               180, 48, RED);
    }

    DrawText(songTitle, 20, 40, 20, LIGHTGRAY);
    DrawText(songArtist, 20, 65, 18, DARKGRAY);

    int notesHit = 0;
    for (int i = 0; i < noteCount; i++) {
      if (!notes[i].active)
        notesHit++;
    }
    float progress =
        (noteCount > 0) ? ((float)notesHit / (float)noteCount) : 0.0f;
    Rectangle barBounds = {40.0f, 8.0f, (float)(GetScreenWidth() - 80), 24.0f};
    GuiProgressBar(barBounds, "", "", &progress, 0.0f, 1.0f);

    DrawButtons();
    DrawNotes();

    EndDrawing();
  }
}
