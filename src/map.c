#include "map.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

char songTitle[128] = "Unknown";
char songArtist[128] = "Unknown";
int resolution = 192;
int tempo = 500000;
float offset = 0.0f;

extern Note notes[MAX_NOTES];
extern int noteCount;

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
