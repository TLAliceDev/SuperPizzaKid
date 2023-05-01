#include "../alpaca.h"

#include <stdlib.h>
#include <stdio.h>
#include <yaml.h>

// TODO: implement sequence reading

struct alpaca_node_s {
  char* value;
  struct alpaca_node_s* child;
  struct alpaca_node_s* next;
  struct alpaca_node_s* parent;
};


static alpaca_node_t*
alpaca_node_generate(yaml_parser_t* parser) {
  yaml_token_t token;
  alpaca_node_t* root = calloc(1,sizeof(*root));
  alpaca_node_t* node = root;
  root->value = "ROOT";
  int previous_state = YAML_DOCUMENT_START_TOKEN;
  int indent = 0;
  do {
    yaml_parser_scan(parser,&token);
    switch (token.type) {
      case YAML_KEY_TOKEN:
        if (node->value) {
          node->next = calloc(1,sizeof(*node));
          node->next->parent = node->parent;
          node = node->next;
        }
        //printf("Key: ");
        break;
      case YAML_VALUE_TOKEN:
        node->child = calloc(1,sizeof(*node));
        node->child->parent = node;
        node = node->child;
        //printf("Value: ");
        break;
      case YAML_BLOCK_SEQUENCE_START_TOKEN:
        //indent++;
        //printf("Sequence Start\n");
        break;
      case YAML_BLOCK_ENTRY_TOKEN:
        //printf("Entry\n");
        break;
      case YAML_BLOCK_END_TOKEN:
        node = node->parent;
        //indent--;
        //printf("Block End\n");
        break;
      case YAML_BLOCK_MAPPING_START_TOKEN:
        //indent++;
        //printf("Mapping start\n");
        break;
      case YAML_SCALAR_TOKEN:
        node->value = calloc(strlen(token.data.scalar.value)+1,sizeof(char));
        strcpy(node->value,token.data.scalar.value);
        if (previous_state == YAML_VALUE_TOKEN)
          node = node->parent;
        //printf("%s\n", token.data.scalar.value);
        break;
    }
    //for (int i = 0; i < indent; i++)
    //  printf("  ");
    previous_state = token.type;
    if (token.type != YAML_STREAM_END_EVENT)
      yaml_token_delete(&token);
  } while (token.type != YAML_STREAM_END_EVENT);
  return root;
}

alpaca_node_t*
alpaca_node_generate_from_file(const char* filepath) {
  alpaca_node_t* node;
  yaml_parser_t parser;
  FILE* file = fopen(filepath, "rb");
  yaml_parser_initialize(&parser);
  yaml_parser_set_input_file(&parser,file);
  node = alpaca_node_generate(&parser);
  yaml_parser_delete(&parser);
  return node;
}

alpaca_node_t*
alpaca_node_find(alpaca_node_t* root, const char* key) {
  if (!root)
    return NULL;
  if (!strcmp(root->value,key))
    return root;
  alpaca_node_t* new_root = NULL;
  if (root->child)
    new_root = alpaca_node_find(root->child,key);
  if (!new_root && root->next)
    new_root = alpaca_node_find(root->next,key);
  return new_root;
}

alpaca_node_t*
alpaca_node_find_child(alpaca_node_t* root, const char* key) {
  if (!root)
    return NULL;
  if (!strcmp(root->value,key))
    return root->child;
  alpaca_node_t* new_root = NULL;
  if (root->child)
    new_root = alpaca_node_find_child(root->child,key);
  if (!new_root && root->next)
    new_root = alpaca_node_find_child(root->next,key);
  return new_root;
}

alpaca_node_t*
alpaca_node_child(alpaca_node_t* root) {
  return root->child;
}

alpaca_node_t*
alpaca_node_next(alpaca_node_t* root) {
  return root->next;
}

void
alpaca_node_destroy(alpaca_node_t* root) {
  if (root->child)
    alpaca_node_destroy(root->child);
  if (root->next)
    alpaca_node_destroy(root->next);
  free(root);
}

void
alpaca_node_read_value(alpaca_node_t* node, char** buffer) {
  *buffer = calloc(strlen(node->value)+1,sizeof(**buffer));
  strcpy(*buffer,node->value);
}

int
alpaca_node_read_value_int(alpaca_node_t* node) {
  return atoi(node->value);
}

int
alpaca_node_read_value_float(alpaca_node_t* node) {
  return atoi(node->value);
}

void
alpaca_node_print(alpaca_node_t* node, int indentation_level) {
  if (!node)
    return;
  for (int i = 0; i < indentation_level; i++)
    printf("  ");
  printf(".");
  if (node->value)
    printf("%s",node->value);
  printf("\n");
  alpaca_node_print(node->child,indentation_level+1);
  alpaca_node_print(node->next,indentation_level);
}
