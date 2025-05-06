#include "init.h"
#include "map.h"
#include "types.h"
#include <raylib.h>

void GameInit(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C Game");
  SetTargetFPS(60);
  LoadMap("assets/song1.chart");
}
