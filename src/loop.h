#ifndef LOOP_H
#define LOOP_H

typedef enum {
  STATE_MENU,
  STATE_LEVEL_SELECT,
  STATE_PLAYING,
  STATE_PAUSED,
  STATE_RESUME,
  STATE_RESULTS
} GameState;

void GameLoop(void);

#endif  // LOOP_H
