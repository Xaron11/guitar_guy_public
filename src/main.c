#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_COLUMNS 4
#define NOTE_RADIUS 20
#define HIT_WINDOW 30
#define MAX_NOTES 2048
#define HIT_FEEDBACK_TIME 0.2f
#define KEY_DELAY 0.2f

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const Color BUTTON_COLORS[NUM_COLUMNS] = {GREEN, RED, BLUE, YELLOW};
const int BUTTON_KEYS[NUM_COLUMNS] = {KEY_A, KEY_S, KEY_D, KEY_F};

typedef struct Note {
  int column;
  float time;
  bool active;
} Note;

Note notes[MAX_NOTES];
int noteCount = 0;
int score = 0;
float songTime = 0.0f;

int resolution = 192;
int tempo = 500000;
float offset = 0.0f;

float keyTimers[NUM_COLUMNS] = {0};
bool keyHitVisual[NUM_COLUMNS] = {false};
float keyHitTimers[NUM_COLUMNS] = {0};

char songTitle[128] = "Unknown";
char songArtist[128] = "Unknown";

void LoadMap(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    TraceLog(LOG_ERROR, "Failed to open map file");
    return;
  }
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, "Offset"))
      sscanf(line, "Offset = %f", &offset);
    else if (strstr(line, "Resolution"))
      sscanf(line, "Resolution = %d", &resolution);
    else if (strstr(line, "Tempo"))
      sscanf(line, "Tempo = %d", &tempo);
    else if (strstr(line, "Name"))
      sscanf(line, "Name = \"%99[^\"]\"", songTitle);
    else if (strstr(line, "Artist"))
      sscanf(line, "Artist = \"%99[^\"]\"", songArtist);
    else if (strstr(line, "= N")) {
      int tick = 0, column = 0;
      if (sscanf(line, "%d = N %d", &tick, &column) == 2) {
        if (noteCount < MAX_NOTES && column >= 0 && column < NUM_COLUMNS) {
          float beat = (float)tick / resolution;
          float seconds = (tempo / 1000000.0f) * beat + offset;
          notes[noteCount++] = (Note){column, seconds, true};
        }
      }
    }
  }
  fclose(file);
}

void DrawColumnLines() {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    int x = (i + 1) * SCREEN_WIDTH / (NUM_COLUMNS + 1);
    DrawLine(x, 0, x, SCREEN_HEIGHT, GRAY);
  }
}

void DrawButtons() {
  for (int i = 0; i < NUM_COLUMNS; i++) {
    int x = (i + 1) * SCREEN_WIDTH / (NUM_COLUMNS + 1);
    Color c = BUTTON_COLORS[i];
    if (IsKeyDown(BUTTON_KEYS[i]))
      c = Fade(c, 0.5f);
    if (keyHitVisual[i])
      c = WHITE;
    Vector2 buttonPos = {x, SCREEN_HEIGHT - 50};
    DrawRing(buttonPos, NOTE_RADIUS, NOTE_RADIUS + 5, 0, 360, 32, c);
  }
}

void DrawNotes() {
  for (int i = 0; i < noteCount; i++) {
    if (!notes[i].active)
      continue;
    float noteY = SCREEN_HEIGHT - 50 - ((notes[i].time - songTime) * 300);
    if (noteY > SCREEN_HEIGHT || noteY < -NOTE_RADIUS)
      continue;
    int x = (notes[i].column + 1) * SCREEN_WIDTH / (NUM_COLUMNS + 1);
    DrawCircleLines(x, noteY, NOTE_RADIUS, BUTTON_COLORS[notes[i].column]);
  }
}

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

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C Game");
  SetTargetFPS(60);
  LoadMap("assets/song1.chart");

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

  CloseWindow();
  return 0;
}
