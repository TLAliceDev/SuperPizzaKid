#include "../raccoon.h"

#include "../alpaca.h"

#define RESOURCES_FILE "config/resources.yaml"

static RST
raccoon_parse_resource_type(char *type) {
    if (!strcmp(type,"TEXTURE")) {
      return RST_TEXTURE;
    }
    if (!strcmp(type,"TEXTURE_ATLAS")) {
      return RST_TEXTURE_ATLAS;
    }
    if (!strcmp(type,"SOUND")) {
      return RST_SOUND;
    }
    if (!strcmp(type,"MUSIC")) {
      return RST_MUSIC;
    }
    return RST_INVALID;
}

raccoon_resource_header_t*
raccoon_resource_create_texture(alpaca_node_t* node) {
  raccoon_resource_texture_t* texture = calloc(1,sizeof(*texture));
  raccoon_resource_header_t* header = &texture->header;
  alpaca_node_read_value(node,&header->identifier);
  alpaca_node_read_value(alpaca_node_find_child(node,"filepath"),&header->filepath);
  header->type = RST_TEXTURE;
  return (raccoon_resource_header_t*)texture;
}

raccoon_resource_header_t*
raccoon_resource_create_texture_atlas(alpaca_node_t* node) {
  raccoon_resource_texture_atlas_t* texture_atlas = calloc(1,sizeof(*texture_atlas));
  raccoon_resource_header_t* header = &texture_atlas->header;
  texture_atlas->atlas = calloc(1,sizeof(*texture_atlas->atlas));
  alpaca_node_read_value(node,&header->identifier);
  alpaca_node_read_value(alpaca_node_find_child(node,"filepath"),&header->filepath);
  header->type = RST_TEXTURE_ATLAS;
  texture_atlas->atlas->rect_w = alpaca_node_read_value_int(alpaca_node_find_child(node,"rect_height"));
  texture_atlas->atlas->rect_h = alpaca_node_read_value_int(alpaca_node_find_child(node,"rect_width"));
  return (raccoon_resource_header_t*)texture_atlas;
}

raccoon_resource_header_t*
raccoon_resource_create_sound(alpaca_node_t* node) {
  raccoon_resource_sound_t* sound = calloc(1,sizeof(*sound));
  raccoon_resource_header_t* header = &sound->header;
  alpaca_node_read_value(node,&header->identifier);
  alpaca_node_read_value(alpaca_node_find_child(node,"filepath"),&header->filepath);
  header->type = RST_SOUND;
  return (raccoon_resource_header_t*)sound;
}

raccoon_resource_header_t*
raccoon_resource_create_music(alpaca_node_t* node) {
  raccoon_resource_music_t* music = calloc(1,sizeof(*music));
  raccoon_resource_header_t* header = &music->header;
  alpaca_node_read_value(node,&header->identifier);
  alpaca_node_read_value(alpaca_node_find_child(node,"filepath"),&header->filepath);
  header->type = RST_MUSIC;
  return (raccoon_resource_header_t*)music;
}

raccoon_resource_header_t*
raccoon_resource_create(alpaca_node_t* node) {
  char *type;
  int parsed_type;
  raccoon_resource_header_t* header;
  alpaca_node_read_value(alpaca_node_find_child(node,"type"),&type);
  parsed_type = raccoon_parse_resource_type(type);
  free(type);
  switch (parsed_type) {
    case RST_TEXTURE:
        return raccoon_resource_create_texture(node);
    case RST_TEXTURE_ATLAS:
        return raccoon_resource_create_texture_atlas(node);
    case RST_SOUND:
        return raccoon_resource_create_sound(node);
    case RST_MUSIC:
        return raccoon_resource_create_music(node);
    default:
      break;
  }
  printf("broken\n");
  return NULL;
}

void
raccoon_resource_manager_load_resource_data(raccoon_resource_manager_t* manager) {
  alpaca_node_t* alpaca_root = alpaca_node_generate_from_file(RESOURCES_FILE);
  alpaca_node_t* current = alpaca_node_find(alpaca_root,"resources");
  current = alpaca_node_child(current);
  raccoon_resource_header_t* previous_header = NULL;

  do {
    char *buffer;
    alpaca_node_read_value(current,&buffer);
    raccoon_resource_header_t* new = raccoon_resource_create(current);
    if (previous_header)
      previous_header->next = new;
    else
      manager->first = new;
    previous_header = new;
  } while (current = alpaca_node_next(current));

  alpaca_node_destroy(alpaca_root);

}
