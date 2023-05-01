#ifndef ATLAS_H
#define ATLAS_H
#include <SDL2/SDL.h>

typedef struct atlas_s {
  SDL_Texture* texture;
  uint rect_w;
  uint rect_h;
} atlas_t;

atlas_t*
atlas_atlas_load(SDL_Renderer* renderer, const char* filepath,
                 uint divisions_x, uint divisions_y);

void
atlas_atlas_unload(atlas_t* atlas);

void
atlas_atlas_render(SDL_Renderer* renderer, atlas_t* atlas,
                   int x, int y,
                   uint rx, uint ry);

void
atlas_atlas_render_ex(SDL_Renderer* renderer, atlas_t* atlas,
                   int x, int y,
                   uint rx, uint ry,
                   double angle, double scale_x, double scale_y,
                   int flip_x, int flip_y);

int
atlas_atlas_divisions_x_get(atlas_t* atlas);

int
atlas_atlas_divisions_y_get(atlas_t* atlas);

#endif
