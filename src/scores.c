#include "scores.h"
#include <stdio.h>
#include <string.h>

void LoadHighscores(HighscoreList *list, const char *filename) {
  list->count = 0;
  FILE *f = fopen(filename, "r");
  if (!f)
    return;
  char line[512];
  while (fgets(line, sizeof(line), f)) {
    char fname[256];
    int score;
    if (sscanf(line, "%255[^,],%d", fname, &score) == 2) {
      strncpy(list->entries[list->count].filename, fname, 255);
      list->entries[list->count].filename[255] = '\0';
      list->entries[list->count].highscore = score;
      list->count++;
      if (list->count >= MAX_SONGS)
        break;
    }
  }
  fclose(f);
}

void SaveHighscores(const HighscoreList *list, const char *filename) {
  FILE *f = fopen(filename, "w");
  if (!f)
    return;
  for (int i = 0; i < list->count; i++) {
    fprintf(f, "%s,%d\n", list->entries[i].filename,
            list->entries[i].highscore);
  }
  fclose(f);
}

int GetHighscore(const HighscoreList *list, const char *filename) {
  for (int i = 0; i < list->count; i++) {
    if (strcmp(list->entries[i].filename, filename) == 0) {
      return list->entries[i].highscore;
    }
  }
  return 0;
}

void SetHighscore(HighscoreList *list, const char *filename, int score) {
  for (int i = 0; i < list->count; i++) {
    if (strcmp(list->entries[i].filename, filename) == 0) {
      if (score > list->entries[i].highscore) {
        list->entries[i].highscore = score;
      }
      return;
    }
  }
  // Not found, add new
  if (list->count < MAX_SONGS) {
    strncpy(list->entries[list->count].filename, filename, 255);
    list->entries[list->count].filename[255] = '\0';
    list->entries[list->count].highscore = score;
    list->count++;
  }
}
