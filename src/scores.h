#ifndef SCORES_H
#define SCORES_H
#include "types.h"

void LoadHighscores(HighscoreList *list, const char *filename);
void SaveHighscores(const HighscoreList *list, const char *filename);
int GetHighscore(const HighscoreList *list, const char *filename);
void SetHighscore(HighscoreList *list, const char *filename, int score);

#endif  // SCORES_H
