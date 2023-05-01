#include "../atlas.h"

#include <stdint.h>
#include <SDL2/SDL_image.h>

atlas_t*
atlas_atlas_load(SDL_Renderer* renderer, const char* filepath,
                 uint divisions_x, uint divisions_y) {
  uint texture_width;
  uint texture_height;
  uint rect_w;
  uint rect_h;
  atlas_t* atlas = malloc(sizeof(*atlas));
  SDL_Texture* texture = IMG_LoadTexture(renderer, filepath);
  SDL_QueryTexture(atlas->texture, NULL, NULL, &texture_width, &texture_height);
  rect_w = texture_width/divisions_x;
  rect_h = texture_height/divisions_y;
  atlas->texture = texture;
  atlas->rect_w = rect_w;
  atlas->rect_h = rect_h;
}

void
atlas_atlas_unload(atlas_t* atlas) {
  SDL_DestroyTexture(atlas->texture);
  free(atlas);
}

void
atlas_atlas_render(SDL_Renderer* renderer, atlas_t* atlas,
                   int x, int y, uint rx, uint ry) {
  SDL_Rect src_rect;
  SDL_Rect dst_rect = {x, 
                       y,
                       atlas->rect_w,
                       atlas->rect_h};
  src_rect.x = rx * atlas->rect_w;
  src_rect.y = ry * atlas->rect_h;
  src_rect.w = atlas->rect_w;
  src_rect.h = atlas->rect_w;
  SDL_RenderCopy(renderer, atlas->texture, &src_rect, &dst_rect);
}

void
atlas_atlas_render_ex(SDL_Renderer* renderer, atlas_t* atlas,
                   int x, int y, uint rx, uint ry,
                   double angle, double scale_x, double scale_y,
                   int flip_x, int flip_y) {
  SDL_Rect src_rect;
  SDL_Rect dst_rect = {(x) * scale_x,
                       (y) * scale_y, 
                       atlas->rect_w * scale_x, 
                       atlas->rect_h * scale_y};
  src_rect.x = rx * atlas->rect_w;
  src_rect.y = ry * atlas->rect_h;
  src_rect.w = atlas->rect_w;
  src_rect.h = atlas->rect_w;
  SDL_RenderCopyEx(renderer, atlas->texture,
                   &src_rect, &dst_rect, 
                   angle, NULL, 
                   (SDL_FLIP_HORIZONTAL * flip_x) | (SDL_FLIP_VERTICAL* flip_y));
}

int
atlas_atlas_divisions_x_get(atlas_t* atlas) {
  uint texture_width;
  SDL_QueryTexture(atlas->texture, NULL, NULL, &texture_width, NULL);
  return texture_width / atlas->rect_w;
}

int
atlas_atlas_divisions_y_get(atlas_t* atlas) {
  uint texture_height;
  SDL_QueryTexture(atlas->texture, NULL, NULL, NULL, &texture_height);
  return texture_height / atlas->rect_h;
}
