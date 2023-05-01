#ifndef LEVELS_H
#define LEVELS_H
#include "tilemap.h"

typedef struct level_level_s {
  tilemap_t* tilemap;
  char* level_name;
  float spawn_x;
  float spawn_y;
  struct level_level_s* prev;
  struct level_level_s* next;
} level_level_t;

typedef struct level_level_list_s {
  level_level_t* level;
} level_level_list_t;

level_level_list_t*
level_level_list_create();

#endif
