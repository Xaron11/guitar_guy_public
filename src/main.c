#include "cleanup.h"
#include "init.h"
#include "loop.h"

int main(void) {
  GameInit();
  GameLoop();
  GameCleanup();
  return 0;
}
