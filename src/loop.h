#ifndef LOOP_H
#define LOOP_H

typedef enum {
  STATE_MENU,
  STATE_PLAYING,
  STATE_PAUSED,
  STATE_RESUME
} GameState;

void GameLoop(void);

#endif  // LOOP_H
