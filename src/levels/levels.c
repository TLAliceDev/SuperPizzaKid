#include "../levels.h"
#include "../alpaca.h"
#include "../raccoon.h"

#define LEVELS_FILE "config/levels.yaml"


level_level_list_t*
level_level_list_create() {
  level_level_list_t* list = calloc(1,sizeof(*list));
  alpaca_node_t* root = alpaca_node_generate_from_file(LEVELS_FILE);
  alpaca_node_t* current = alpaca_node_find_child(root,"levels");
  level_level_t* prev = NULL;

  do {
    char* buffer;
    level_level_t* level = calloc(1,sizeof(*level));
    alpaca_node_read_value(current,&level->level_name);
    alpaca_node_read_value(alpaca_node_find_child(current,"tile_data"),&buffer);
    level->tilemap = tilemap_tilemap_create_from_file(buffer);
    level->spawn_x = alpaca_node_read_value_float(alpaca_node_find_child(current,"player_x"));
    level->spawn_y = alpaca_node_read_value_float(alpaca_node_find_child(current,"player_y"));
    if (!prev)
      list->level = level;
    else
      prev->next = level;
    level->prev = prev;
    prev = level;
    free(buffer);
  } while(current = alpaca_node_next(current));

  alpaca_node_destroy(root);

  return list;
}
