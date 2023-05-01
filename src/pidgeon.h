#ifndef PIDGEON_H
#define PIDGEON_H
#include <SDL2/SDL.h>
#include "atlas.h"

void
pidgeon_draw_text(SDL_Renderer* renderer, atlas_t* font, char* text, float x, float y);

#endif
