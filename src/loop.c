#include "loop.h"
#include "draw.h"
#include "game.h"
#include "types.h"
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
    DrawText(songTitle, 10, 40, 20, WHITE);
    DrawText(songArtist, 10, 60, 20, GRAY);
    DrawButtons();
    DrawNotes();

    EndDrawing();
  }
}
