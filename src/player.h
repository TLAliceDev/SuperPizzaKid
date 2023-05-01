#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "atlas.h"
#include "tilemap.h"

typedef struct player_s {
  float x;
  float y;
  float vx;
  float vy;
  float jump_buffer;
  float ground_buffer;

  bool flip;
  int prev_animation;
  int current_animation;
  float animation_timer;
  atlas_t* texture;

  struct {
    Mix_Chunk* jump_sfx;
    Mix_Chunk* step_sfx;
    Mix_Chunk* coin_sfx;
    Mix_Chunk* die_sfx;
    Mix_Chunk* win_sfx;
  } sfxs;
  float step_timer;

  bool dead;
  float dead_timer;
  float spawn_x;
  float spawn_y;

  bool level_finished;

} player_t;


void
player_update(player_t* player, tilemap_t* tilemap, float delta_time);

void
player_draw(player_t* player, SDL_Renderer* renderer);

#endif
