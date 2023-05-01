#include "../pidgeon.h"
#include <string.h>

void
pidgeon_draw_text(SDL_Renderer* renderer, atlas_t* font, char* text, float x, float y) {
  int n = strlen(text);
  float draw_x = 0, draw_y = 0;
  int i = 0;
  char character = text[i];
  while (character != '\0') {
    if (character == '\n') {
      draw_y++;
      draw_x = 0;
    }
    else {
      character-=32;
      int font_x_coord = character%16;
      int font_y_coord = character/16;
      int offset_x = draw_x * font->rect_w;
      int offset_y = draw_y * font->rect_h;
      atlas_atlas_render(renderer, font, x+offset_x, y+offset_y, font_x_coord,font_y_coord);
      draw_x++;
    }
    character = text[++i];
  }
}
