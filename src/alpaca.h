#ifndef ALPACA_H
#define ALPACA_H

typedef struct alpaca_node_s alpaca_node_t;

alpaca_node_t*
alpaca_node_generate_from_file(const char* filepath);

alpaca_node_t*
alpaca_node_find(alpaca_node_t* root, const char* key);

alpaca_node_t*
alpaca_node_find_child(alpaca_node_t* root, const char* key);

alpaca_node_t*
alpaca_node_child(alpaca_node_t* root);

alpaca_node_t*
alpaca_node_next(alpaca_node_t* root);

void
alpaca_node_destroy(alpaca_node_t* root);

void
alpaca_node_read_value(alpaca_node_t* node, char** buffer);

int
alpaca_node_read_value_int(alpaca_node_t* node);

int
alpaca_node_read_value_float(alpaca_node_t* node);

void
alpaca_node_print(alpaca_node_t* node, int indentation_level);

#endif
