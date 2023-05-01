#include "../rat.h"
#include "../alpaca.h"
#include <stdbool.h>

// All of this is deeply bad,
// all of it is going to be rewritten post-jam
// to be used properly, don't scream at me

#define INPUT_FILE "config/inputs.yaml"

struct rat_action_s {
  char* identifier;
  SDL_Scancode keys[8];
  Uint8 active;
  Uint8 active_prev;
};

struct rat_input_system_s {
  rat_action_t actions[32];
};

static rat_input_system_t global_input_system;

void
rat_action_update(rat_action_t* action, SDL_Scancode scancode, bool pressed) {
  action->active_prev = action->active;
  for (int i = 0; i < 8; i++) {
    if (scancode == action->keys[i]) {
      if (pressed)
        action->active |= 1 << i;
      else
        action->active &= ~(1 << i);
    }
  }
}

void
rat_action_frame_update(rat_action_t* action) {
  action->active_prev = action->active;
}

void
rat_input_system_global_update(SDL_Scancode scancode, bool pressed) {
  for (int i = 0; i < 32; i++)
    rat_action_update(&global_input_system.actions[i], scancode, pressed);
}

void
rat_input_system_global_frame_update() {
  for (int i = 0; i < 32; i++)
    rat_action_frame_update(&global_input_system.actions[i]);
}

bool
rat_is_action_pressed(rat_action_t* action) {
  return action->active != 0;
}

bool
rat_is_action_just_pressed(rat_action_t* action) {
  return (action->active != 0) && (action->active_prev == 0);
}

bool
rat_is_action_just_released(rat_action_t* action) {
  return (action->active == 0) && (action->active_prev != 0);
}

bool
rat_is_action_pressed_global(char* identifier) {
  for (int i = 0; i < 32; i++)
    if (!strcmp(identifier,global_input_system.actions[i].identifier))
      return rat_is_action_pressed(&global_input_system.actions[i]);
  return false;
}

bool
rat_is_action_just_pressed_global(char* identifier) {
  for (int i = 0; i < 32; i++)
    if (!strcmp(identifier,global_input_system.actions[i].identifier))
      return rat_is_action_just_pressed(&global_input_system.actions[i]);
  return false;
}

bool
rat_is_action_just_released_global(char* identifier) {
  for (int i = 0; i < 32; i++)
    if (!strcmp(identifier,global_input_system.actions[i].identifier))
      return rat_is_action_just_released(&global_input_system.actions[i]);
  return false;
}

void
rat_input_system_global_init() {
  alpaca_node_t* root = alpaca_node_generate_from_file(INPUT_FILE);
  alpaca_node_t* current = alpaca_node_find_child(root,"inputs");

  int current_id = 0;

  do {
    char* buffer;
    SDL_Scancode scancode;
    rat_action_t* action = &global_input_system.actions[current_id];

    alpaca_node_read_value(current,&action->identifier);

    alpaca_node_read_value(alpaca_node_child(current),&buffer);
    
    action->keys[0] = SDL_GetScancodeFromName(buffer);
    
    free(buffer);

    current_id++;
    
  } while (current = alpaca_node_next(current));

  alpaca_node_destroy(root);
  
}
