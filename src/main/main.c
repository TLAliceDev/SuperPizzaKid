#include <SDL2/SDL.h>
#include "../boar.h"
#ifndef WEB_BUILD
void
game_loop(boar_game_state_t* game_state) {
  while (!boar_game_state_update(game_state))
    boar_game_state_draw(game_state);
}

int
main() {
  boar_game_state_t* game_state = boar_game_state_create();
  game_loop(game_state);
  boar_game_state_destroy(game_state);
}
#else
void
game_loop() {
  static boar_game_state_t* game_state = boar_game_state_create();
  boar_game_state_update(game_state);
  boar_game_state_draw(game_state);
}
int
main() {
  emscripten_set_main_loop(game_loop,0,1);
}
#endif
