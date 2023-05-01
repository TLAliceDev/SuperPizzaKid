#include  "../tilemap.h"
#include <string.h>
#include "../owl.h"

tilemap_t*
tilemap_tilemap_create(Uint8 tiles[15][16], atlas_t* texture) {
  tilemap_t* new_tilemap = calloc(1,sizeof(*new_tilemap));
  if (tiles) {
    for (int i = 0; i < 15; i++)
      memcpy(new_tilemap->tiles[i],tiles[i],16);
  } 
  else {
    for (int i = 0; i < 15; i++)
      for (int j = 0; j < 16; j++)
        new_tilemap->tiles[i][j] = 0xFF;
  }
  new_tilemap->atlas = texture;
  return new_tilemap;
}

tilemap_t*
tilemap_tilemap_create_from_file(char* filename) {
  FILE* file = fopen(filename,"rb");
  tilemap_t* new_tilemap = calloc(1,sizeof(*new_tilemap));
  for (int i = 0; i < 15; i++)
    fread(new_tilemap->tiles[i],sizeof(Uint8),16,file);
  fclose(file);
  return new_tilemap;
}

void
tilemap_tilemap_render(SDL_Renderer* renderer, tilemap_t* tilemap) {
  for(int i = 0; i < 15; i++) {
    for(int j = 0; j < 16; j++) {
      Uint8 tile = tilemap->tiles[i][j];
      if (tile == 0xFF)
        continue;
      int y = tile/4;
      int x = tile%4;
      atlas_atlas_render(renderer, tilemap->atlas,j*16,i*16,x,y);
    }
  }
}

void
tilemap_tilemap_set_tile(tilemap_t* tilemap, int x, int y, int tile) {
  tilemap->tiles[y][x] = tile;
}

int
tilemap_tilemap_intersects(tilemap_t* tilemap, float x, float y, float w, float h, int *int_x, int *int_y) {
  int min_x = x/16;
  int min_y = y/16;
  int max_x = (x+w)/16;
  int max_y = (y+h)/16;
  
  for (int i = min_x; i <= max_x; i++)
    for (int j = min_y; j <= max_y; j++)
      if ((tilemap->tiles[j][i]+1)%4 != 0) {
        if (int_x)
          *int_x = i;
        if (int_y)
          *int_y = j;
        return tilemap->tiles[j][i];
      }
  return 0xFF;
}
