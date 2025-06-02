#include "draw.h"
#include "game.h"
#include "raygui.h"
#include "scores.h"
#include "types.h"
#include <raylib.h>
#include <stdio.h>

void DrawColumnLines() {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    int x = (i + 1) * SCREEN_WIDTH / (NUM_COLUMNS + 1);
    DrawLine(x, 0, x, SCREEN_HEIGHT, GRAY);
  }
}

void DrawButtons(const GameStateData *state, bool enableVisuals) {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    float x = (float)((i + 1) * SCREEN_WIDTH) / (NUM_COLUMNS + 1);
    Color c = BUTTON_COLORS[i];
    if (enableVisuals) {
      if (IsKeyDown(BUTTON_KEYS[i]))
        c = Fade(c, 0.5f);
      if (state->keyHitVisual[i])
        c = WHITE;
    }
    Vector2 buttonPos = {x, (float)(SCREEN_HEIGHT - 50)};
    DrawRing(buttonPos, NOTE_RADIUS, NOTE_RADIUS + 5, 0, 360, 32, c);
  }
}

void DrawNotes(const GameStateData *state) {
  for (int i = 0; i < state->currentSong.noteCount; i++) {
    if (!state->currentSong.notes[i].active)
      continue;
    float noteY =
        (float)SCREEN_HEIGHT - 50 -
        ((state->currentSong.notes[i].time - state->songTime) * 300.0f);
    if (noteY > (float)SCREEN_HEIGHT || noteY < -NOTE_RADIUS)
      continue;
    float x = (float)((state->currentSong.notes[i].column + 1) * SCREEN_WIDTH) /
              (NUM_COLUMNS + 1);
    DrawCircleLines((int)x, (int)noteY, NOTE_RADIUS,
                    BUTTON_COLORS[state->currentSong.notes[i].column]);
  }
}

void DrawScoreUI(const GameStateData *state) {
  DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 24) / 2, 40, 24,
           GRAY);
  DrawText(TextFormat("%d", state->score),
           GetScreenWidth() / 2 -
               MeasureText(TextFormat("%d", state->score), 64) / 2,
           70, 64, YELLOW);
  if (state->combo > 0) {
    DrawText(TextFormat("COMBO: %d", state->combo),
             GetScreenWidth() / 2 -
                 MeasureText(TextFormat("COMBO: %d", state->combo), 36) / 2,
             140, 36, ORANGE);
  }
  if (state->multiplier > 1) {
    DrawText(TextFormat("x%d", state->multiplier),
             GetScreenWidth() / 2 -
                 MeasureText(TextFormat("x%d", state->multiplier), 48) / 2,
             180, 48, RED);
  }
}

void DrawSongInfo(const GameStateData *state) {
  DrawText(state->currentSong.title, 20, 40, 20, LIGHTGRAY);
  DrawText(state->currentSong.artist, 20, 65, 18, DARKGRAY);
}

void DrawProgressBar(const GameStateData *state) {
  float progress = GetSongProgress(state);
  Rectangle barBounds = {40.0f, 8.0f, (float)(GetScreenWidth() - 80), 24.0f};

  GuiProgressBar(barBounds, "", "", &progress, 0.0f, 1.0f);
}

void DrawPauseOverlay(void) {
  bool dummy = false;
  DrawPauseOverlayWithExit(&dummy);
}

void DrawPauseOverlayWithExit(bool *exitPressed) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
  DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 64) / 2,
           GetScreenHeight() / 2 - 32, 64, YELLOW);
  DrawText("Press ESC to resume",
           GetScreenWidth() / 2 - MeasureText("Press ESC to resume", 24) / 2,
           GetScreenHeight() / 2 + 40, 24, GRAY);
  *exitPressed = GuiButton(
      (Rectangle){10.0f, (float)(GetScreenHeight() - 60), 200.0f, 50.0f},
      "Exit to Menu");
}

void DrawResumeCountdown(int seconds) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", seconds);
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
  DrawText(buf, GetScreenWidth() / 2 - MeasureText(buf, 96) / 2,
           GetScreenHeight() / 2 - 48, 96, YELLOW);
}

void DrawMainMenu(bool *playPressed, bool *exitPressed) {
  const char *title = "gUITAR gUY";
  int titleFontSize = 64;
  int btnWidth = 200;
  int btnHeight = 60;
  int btnX = GetScreenWidth() / 2 - btnWidth / 2;
  int btnY = GetScreenHeight() / 2 - btnHeight / 2 + 60;
  int exitBtnY = btnY + btnHeight + 30;
  DrawText(title, GetScreenWidth() / 2 - MeasureText(title, titleFontSize) / 2,
           100, titleFontSize, YELLOW);
  DrawText("Dawid Kaczmarzyk & Mateusz Borucki",
           GetScreenWidth() / 2 -
               MeasureText("Dawid Kaczmarzyk & Mateusz Borucki", 24) / 2,
           180, 24, GRAY);

  int prevStyle = GuiGetStyle(DEFAULT, TEXT_SIZE);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
  *playPressed = GuiButton(
      (Rectangle){(float)btnX, (float)btnY, (float)btnWidth, (float)btnHeight},
      "PLAY");
  *exitPressed = GuiButton((Rectangle){(float)btnX, (float)exitBtnY,
                                       (float)btnWidth, (float)btnHeight},
                           "EXIT");
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevStyle);
}

typedef struct {
  float x;
  float y;
  float w;
  float h;
  const char *line1;
  Color color1;
  const char *line2;
  Color color2;
} LevelSelectButtonParams;

static bool DrawLevelSelectButton(const LevelSelectButtonParams *params,
                                  bool *hovered) {
  Vector2 mouse = GetMousePosition();
  Rectangle rect = {params->x, params->y, params->w, params->h};
  bool isHover = CheckCollisionPointRec(mouse, rect);
  if (hovered)
    *hovered = isHover;
  bool isClick = isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

  Color bg = isHover ? Fade(GetColor(0xFFD700FF), 0.8f)
                     : Fade(GetColor(0xFFFF00FF), 0.9f);
  Color border = isHover ? LIGHTGRAY : DARKGRAY;
  DrawRectangleRounded(rect, 0.22f, 8, bg);
  DrawRectangleRoundedLines(rect, 0.22f, 8, border);

  int fontSize1 = 22;
  int fontSize2 = 18;
  int text1W = MeasureText(params->line1, fontSize1);
  int text2W = MeasureText(params->line2, fontSize2);
  int text1X = (int)(params->x + params->w / 2.0f - (float)text1W / 2.0f);
  int text2X = (int)(params->x + params->w / 2.0f - (float)text2W / 2.0f);
  int text1Y = (int)(params->y + params->h / 2.0f - (float)fontSize1 + 2.0f);
  int text2Y = (int)(params->y + params->h / 2.0f + 8.0f);
  DrawText(params->line1, text1X, text1Y, fontSize1, params->color1);
  DrawText(params->line2, text2X, text2Y, fontSize2, params->color2);
  return isClick;
}

void DrawLevelSelectMenu(const GameContext *ctx, const char **songNames,
                         int songCount, int *selectedIdx, bool *backPressed) {
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();
  int btnW = 400;
  int btnH = 90;
  int spacing = 16;
  int totalH = songCount * (btnH + spacing) - spacing;
  int baseY = screenH / 2 - totalH / 2;
  *selectedIdx = -1;
  DrawText("Select Song", screenW / 2 - MeasureText("Select Song", 36) / 2, 60,
           36, YELLOW);
  for (int i = 0; i < songCount; i++) {
    float x = (float)screenW / 2.0f - (float)btnW / 2.0f;
    float y = (float)baseY + (float)i * ((float)btnH + (float)spacing);
    const char *songLabel = songNames[i];
    int highscore =
        GetHighscore(&ctx->highscores, ctx->songList.entries[i].path);
    char scoreLabel[64];
    snprintf(scoreLabel, sizeof(scoreLabel), "Highscore: %d", highscore);
    bool hovered = false;
    LevelSelectButtonParams btn = {x,         y,     (float)btnW, (float)btnH,
                                   songLabel, BLACK, scoreLabel,  DARKGRAY};
    if (DrawLevelSelectButton(&btn, &hovered)) {
      *selectedIdx = i;
    }
  }
  if (GuiButton((Rectangle){(float)screenW / 2.0f - 100.0f,
                            (float)screenH - 80.0f, 200.0f, 40.0f},
                "Back")) {
    *backPressed = true;
  }
}

void DrawResultsScreen(const GameStateData *state, bool *menuPressed,
                       int highscore) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
  int centerX = GetScreenWidth() / 2;
  int y = 120;
  DrawText("SONG COMPLETE!", centerX - MeasureText("SONG COMPLETE!", 40) / 2, y,
           40, YELLOW);
  y += 60;
  DrawText(TextFormat("Score: %d", state->score),
           centerX - MeasureText(TextFormat("Score: %d", state->score), 32) / 2,
           y, 32, WHITE);
  y += 40;
  DrawText(TextFormat("Max Combo: %d", state->maxCombo),
           centerX -
               MeasureText(TextFormat("Max Combo: %d", state->maxCombo), 28) /
                   2,
           y, 28, ORANGE);
  y += 40;
  DrawText(TextFormat("Highscore: %d", highscore),
           centerX -
               MeasureText(TextFormat("Highscore: %d", highscore), 28) / 2,
           y, 28, GREEN);
  y += 40;
  Rectangle btn = {(float)(centerX - 100), (float)y, 200.0f, 50.0f};
  *menuPressed = GuiButton(btn, "Return to Menu");
}
