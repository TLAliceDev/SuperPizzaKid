#ifndef RACCOON_H
#define RACCOON_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "atlas.h"
#include "boar.h"
#include "tilemap.h"

typedef enum {
  RST_INVALID,
  RST_TEXTURE,
  RST_TEXTURE_ATLAS,
  RST_SOUND,
  RST_MUSIC,
  RST_COUNT
} RST;

typedef struct raccoon_resource_header_s {
  RST type;
  const char* identifier;
  const char* filepath;
  struct raccoon_resource_header_s* next;
} raccoon_resource_header_t;

typedef struct raccoon_resource_manager {
  raccoon_resource_header_t* first;
} raccoon_resource_manager_t;

typedef struct raccoon_resource_texture_s {
  raccoon_resource_header_t header;
  SDL_Texture* texture;
} raccoon_resource_texture_t;

typedef struct raccoon_resource_sound_s {
  raccoon_resource_header_t header;
  Mix_Chunk* sound;
} raccoon_resource_sound_t;

typedef struct raccoon_resource_music_s {
  raccoon_resource_header_t header;
  Mix_Music* music;
} raccoon_resource_music_t;

typedef struct raccoon_resource_texture_atlas_s {
  raccoon_resource_header_t header;
  atlas_t* atlas;
} raccoon_resource_texture_atlas_t;

raccoon_resource_manager_t*
raccoon_resource_manager_global_create();

void*
raccoon_resource_manager_get(char* key, boar_game_state_t* game_state);

void
raccoon_resource_manager_load_resource_data(raccoon_resource_manager_t* manager);
#endif
