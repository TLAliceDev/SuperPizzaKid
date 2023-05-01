#ifndef TILEMAP_H
#define TILEMAP_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "atlas.h"

typedef struct tilemap_s {
  Uint8 tiles[15][16];
  atlas_t* atlas;
} tilemap_t;

tilemap_t*
tilemap_tilemap_create(Uint8 tiles[15][16], atlas_t* texture);

tilemap_t*
tilemap_tilemap_create_from_file(char* filename);

void
tilemap_tilemap_render(SDL_Renderer* renderer, tilemap_t* tilemap);

void
tilemap_tilemap_set_tile(tilemap_t* tilemap, int x, int y, int tile);

int
tilemap_tilemap_intersects(tilemap_t* tilemap, float x, float y, float w, float h, int *int_x, int *int_y);
#endif

