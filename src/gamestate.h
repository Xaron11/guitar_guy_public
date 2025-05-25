#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>

#include "loop.h"
#include "types.h"

void ScanAvailableSongs(GameContext *ctx);
const SongList *GetSongList(const GameContext *ctx);

GameState HandleMenuState(const GameContext *ctx, bool *shouldExit);
GameState HandlePlayingState(GameContext *ctx);
GameState HandlePausedState(const GameContext *ctx, int *resumeCountdown,
                            float *resumeTimer);
GameState HandleResumeState(const GameContext *ctx, int *resumeCountdown,
                            float *resumeTimer);
GameState HandleLevelSelectState(const GameContext *ctx, bool *backToMenu,
                                 int *selectedSongIdx);
void SetCurrentSong(GameContext *ctx, int songIdx);

#endif  // GAMESTATE_H
