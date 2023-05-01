#include "../raccoon.h"
#include "../boar.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static const raccoon_resource_manager_t* global_manager;

static SDL_Texture*
raccoon_resource_manager_get_texture(raccoon_resource_texture_t* texture, boar_game_state_t* game_state) {
  if (!texture->texture)
    texture->texture = IMG_LoadTexture(game_state->renderer,texture->header.filepath);
  return texture->texture;
}

static atlas_t*
raccoon_resource_manager_get_texture_atlas(raccoon_resource_texture_atlas_t* atlas, boar_game_state_t* game_state) {
  if (!atlas->atlas->texture)
    atlas->atlas->texture = IMG_LoadTexture(game_state->renderer,atlas->header.filepath);
  return atlas->atlas;
}

static Mix_Chunk*
raccoon_resource_manager_get_sound(raccoon_resource_sound_t* sound) {
  if (!sound->sound)
    sound->sound = Mix_LoadWAV(sound->header.filepath);
  return sound->sound;
}

static Mix_Music*
raccoon_resource_manager_get_music(raccoon_resource_music_t* music) {
  if (!music->music)
    music->music = Mix_LoadMUS(music->header.filepath);
  return music->music;
}

raccoon_resource_manager_t*
raccoon_resource_manager_global_create() {
  if (!global_manager) {
    global_manager = calloc(1,sizeof(*global_manager));
    raccoon_resource_manager_load_resource_data(global_manager);
  }
  return global_manager;
}

void*
raccoon_resource_manager_get(char* key, boar_game_state_t* game_state) {
  raccoon_resource_header_t* header = global_manager->first;
  while (header->next && strcmp(header->identifier,key)) {
    header = header->next;
  }
  if (!header)
    return NULL;
  switch (header->type) {
    case RST_TEXTURE:
      return (void*)raccoon_resource_manager_get_texture((raccoon_resource_texture_t*)header, game_state);
    case RST_TEXTURE_ATLAS:
      return (void*)raccoon_resource_manager_get_texture_atlas((raccoon_resource_texture_atlas_t*)header, game_state);
    case RST_SOUND:
      return (void*)raccoon_resource_manager_get_sound((raccoon_resource_sound_t*)header);
    case RST_MUSIC:
      return (void*)raccoon_resource_manager_get_music((raccoon_resource_music_t*)header);
    default:
      return NULL;
  }
}
