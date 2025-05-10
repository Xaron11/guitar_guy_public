#include "init.h"
#include "map.h"
#include "raygui.h"
#include "types.h"
#include <raylib.h>

void GameInit(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C Game");
  SetTargetFPS(60);
  SetExitKey(0);
  LoadMap("assets/song1.chart");

  // Set global raygui styles
  GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xFFFF00FF);    // YELLOW
  GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xFFD700FF);   // GOLD (hover)
  GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0xFFD766FF);   // dark GOLD (pressed)
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0x222222FF);    // dark gray
  GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x222222FF);   // dark gray
  GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0x222222FF);   // dark gray
  GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x222222FF);  // dark gray
  GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x666666FF); // light gray
  GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x666666FF); // light gray

  GuiSetStyle(PROGRESSBAR, BASE_COLOR_NORMAL, 0xFFFF00FF);    // YELLOW
  GuiSetStyle(PROGRESSBAR, BASE_COLOR_FOCUSED, 0xFFD700FF);   // GOLD (hover)
  GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, 0xFFD700FF);   // GOLD (pressed)
  GuiSetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL, 0x222222FF);  // dark gray
  GuiSetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED, 0x222222FF); // dark gray
  GuiSetStyle(PROGRESSBAR, BORDER_COLOR_PRESSED, 0x222222FF); // dark gray
}
