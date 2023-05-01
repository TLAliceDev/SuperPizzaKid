#ifndef RAT_H
#define RAT_H
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct rat_action_s rat_action_t;

typedef struct rat_input_system_s rat_input_system_t;

void
rat_input_system_global_update(SDL_Scancode scancode, bool pressed);

void
rat_input_system_global_frame_update();

bool
rat_is_action_pressed_global(char* identifier);

bool
rat_is_action_just_pressed_global(char* identifier);

bool
rat_is_action_just_released_global(char* identifier);

void
rat_input_system_global_init();

#endif
