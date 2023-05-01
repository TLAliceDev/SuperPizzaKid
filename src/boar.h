#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "player.h"
#include "levels.h"
#include "tilemap.h"

typedef enum {
  MENU,
  LEVEL,
  LEVEL_EDITOR,
} BOAR_STATES;

typedef struct boar_game_state_s {
  SDL_Window* window;
  SDL_Renderer* renderer;
  atlas_t* font;
  struct time_s {
    Uint64 prev_frame;
    float time_scale;
    float runtime;
    float frame_time;
    float delta_time;
  } time;
  level_level_list_t* level_list;
  level_level_t* current_level;
  union game_data_u {
    struct menu_s {
      SDL_Texture* titlescreen_texture;
    } menu;
    struct level_s {
      player_t player;
      tilemap_t* current_tilemap;
    } level;
    struct level_editor_s {
      tilemap_t* current_tilemap;
      int currently_selected_tile_x;
      int currently_selected_tile_y;
      int tile_to_paint;
      bool selecting_tile;
      char filename[33];
      bool saving;
    } level_editor;
  } game_data;
  BOAR_STATES state;
} boar_game_state_t;

boar_game_state_t*
boar_game_state_create();

void
boar_game_state_destroy(boar_game_state_t* game_state);

bool
boar_game_state_update(boar_game_state_t* game_state);

void
boar_game_state_draw(boar_game_state_t* game_state);

#endif
