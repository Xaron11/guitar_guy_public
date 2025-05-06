#include "loop.h"
#include "draw.h"
#include "game.h"
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
    DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
    DrawText(TextFormat("Combo: %d", GetCombo()), 10, 30, 20, YELLOW);
    DrawText(TextFormat("Multiplier: x%d", GetMultiplier()), 10, 50, 20,
             ORANGE);
    DrawText(songTitle, 10, 80, 20, WHITE);
    DrawText(songArtist, 10, 100, 20, GRAY);
    DrawButtons();
    DrawNotes();

    EndDrawing();
  }
}
