#ifndef GAME_H
#define GAME_H

#include "types.h"

void UpdateInput(GameStateData *state, float delta);
void GameReset(GameStateData *state);
void CalculateSongDuration(Song *song);
float GetSongProgress(const GameStateData *state);
bool IsSongFinished(const GameStateData *state);

#endif  // GAME_H
