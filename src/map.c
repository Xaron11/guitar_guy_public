#include "map.h"
#include "game.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

void LoadMap(const char *filename, Song *song) {
  char *text = LoadFileText(filename);
  if (!text) {
    TraceLog(LOG_ERROR, "Failed to open map file");
    return;
  }
  // Reset song data
  memset(song, 0, sizeof(Song));
  strcpy(song->title, "Unknown");
  strcpy(song->artist, "Unknown");
  song->resolution = 192;
  song->tempo = 500000;
  song->offset = 0.0f;
  song->noteCount = 0;

  int inTrackSection = 0;
  int currNote = 0;
  char *saveptr = NULL;
  const char *line = strtok_r(text, "\n", &saveptr);
  while (line) {
    if (strstr(line, "[Track]")) {
      inTrackSection = 1;
      line = strtok_r(NULL, "\n", &saveptr);
      continue;
    }
    if (inTrackSection && line[0] == '[')
      break;
    if (!inTrackSection) {
      if (strstr(line, "Name"))
        sscanf(line, "Name = \"%99[^\"]\"", song->title);
      else if (strstr(line, "Artist"))
        sscanf(line, "Artist = \"%99[^\"]\"", song->artist);
      else if (strstr(line, "Offset"))
        sscanf(line, "Offset = %f", &song->offset);
      else if (strstr(line, "Resolution"))
        sscanf(line, "Resolution = %d", &song->resolution);
      else if (strstr(line, "Tempo"))
        sscanf(line, "Tempo = %d", &song->tempo);
      else if (strstr(line, "Difficulty"))
        sscanf(line, "Difficulty = %d", &song->difficulty);
      else if (strstr(line, "Notes"))
        sscanf(line, "Notes = %d", &song->noteCount);
    } else if (strstr(line, "= N")) {
      int tick = 0;
      int column = 0;
      if (sscanf(line, "%d = N %d", &tick, &column) == 2 &&
          currNote < song->noteCount && column >= 0 && column < NUM_COLUMNS &&
          song->resolution > 0) {
        float beat = (float)tick / (float)song->resolution;
        float seconds = ((float)song->tempo / 1000000.0f) * beat + song->offset;
        song->notes[currNote++] = (Note){column, seconds, true};
      }
    }
    line = strtok_r(NULL, "\n", &saveptr);
  }

  CalculateSongDuration(song);
  UnloadFileText(text);
}
