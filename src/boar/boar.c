#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "../boar.h"
#include "../raccoon.h"
#include "../rat.h"
#include "../atlas.h"
#include "../tilemap.h"
#include "../owl.h"
#include "../pidgeon.h"

#define GAME_WIDTH 256
#define GAME_HEIGHT 240

void
boar_game_state_level_init(boar_game_state_t* game_state, level_level_t* level) {
  struct level_s* level_data = &game_state->game_data.level;
  player_t* player = &level_data->player;
  level_data->current_tilemap = level->tilemap;
  level_data->current_tilemap->atlas = raccoon_resource_manager_get("tileset",game_state);
  player->vx = 0;
  player->vy = 0;

  player->x = level->spawn_x;
  player->y = level->spawn_y;
  player->spawn_x = level->spawn_x;
  player->spawn_y = level->spawn_y;

  player->texture = raccoon_resource_manager_get("player_texture",game_state);
  player->sfxs.jump_sfx = raccoon_resource_manager_get("jump_sfx",game_state);
  player->sfxs.step_sfx = raccoon_resource_manager_get("step_sfx",game_state);
  player->sfxs.coin_sfx = raccoon_resource_manager_get("coin_sfx",game_state);
  player->sfxs.die_sfx = raccoon_resource_manager_get("die_sfx",game_state);
  player->sfxs.win_sfx = raccoon_resource_manager_get("win_sfx",game_state);

  player->jump_buffer = -1.0;

  player->dead = false;
  player->level_finished = false;

  game_state->state = LEVEL;
}


void
boar_game_state_menu_init(boar_game_state_t* game_state) {
  game_state->game_data.menu.titlescreen_texture = raccoon_resource_manager_get("title_screen",game_state);
  game_state->state = MENU;
}

void
boar_game_state_level_editor_init(boar_game_state_t* game_state) {
  game_state->game_data.level_editor.current_tilemap = game_state->current_level->tilemap;
  if (!game_state->game_data.level_editor.current_tilemap->atlas)
    game_state->game_data.level_editor.current_tilemap->atlas = raccoon_resource_manager_get("tileset",game_state);
  game_state->game_data.level_editor.saving = false;
  game_state->game_data.level_editor.filename[0] = '\0';
  game_state->state = LEVEL_EDITOR;
}

boar_game_state_t*
boar_game_state_create() {
  SDL_Init(SDL_INIT_EVERYTHING);
  boar_game_state_t* game_state = calloc(1,sizeof(*game_state));
  game_state->window = SDL_CreateWindow("LD53!",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        GAME_WIDTH*2, GAME_HEIGHT*2,
                                        SDL_WINDOW_SHOWN |
                                        SDL_WINDOW_RESIZABLE);
  game_state->renderer = SDL_CreateRenderer(game_state->window, -1, SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(game_state->renderer,GAME_WIDTH,GAME_HEIGHT);
  SDL_SetRenderDrawBlendMode(game_state->renderer,SDL_BLENDMODE_BLEND);
  IMG_Init(IMG_INIT_PNG);
  Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
  raccoon_resource_manager_global_create();
  rat_input_system_global_init();
  game_state->time.prev_frame = SDL_GetPerformanceCounter();
  game_state->time.time_scale = 1.0;
  game_state->time.runtime = 0.0;
  game_state->level_list = level_level_list_create();
  game_state->current_level = game_state->level_list->level;
  game_state->font = raccoon_resource_manager_get("pine8x8",game_state);
  boar_game_state_menu_init(game_state);
  Mix_PlayMusic(raccoon_resource_manager_get("level_music",game_state),-1);
  return game_state;
}

void
boar_game_state_destroy(boar_game_state_t* game_state) {
  SDL_DestroyRenderer(game_state->renderer);
  SDL_DestroyWindow(game_state->window);
  IMG_Quit();
  SDL_Quit();
}

void
boar_game_state_update_level(boar_game_state_t* game_state) {
  struct level_s* level = &game_state->game_data.level;
  player_update(&level->player, level->current_tilemap, game_state->time.delta_time);
  if (rat_is_action_just_pressed_global("back_to_menu"))
    boar_game_state_menu_init(game_state);
  if (level->player.level_finished && rat_is_action_just_pressed_global("load_level")) {
    if (game_state->current_level->next) {
      game_state->current_level = game_state->current_level->next;
      boar_game_state_level_init(game_state,game_state->current_level);
    }
    else {
      boar_game_state_menu_init(game_state);
    }
  }
}

void
boar_game_state_update_menu(boar_game_state_t* game_state) {
  if (rat_is_action_just_pressed_global("right") && game_state->current_level->next)
    game_state->current_level = game_state->current_level->next;
  if (rat_is_action_just_pressed_global("left") && game_state->current_level->prev)
    game_state->current_level = game_state->current_level->prev;
  if (rat_is_action_just_pressed_global("load_level"))
    boar_game_state_level_init(game_state,game_state->current_level);
  if (rat_is_action_just_pressed_global("level_editor"))
    boar_game_state_level_editor_init(game_state);
}

void
boar_game_state_update_level_editor(boar_game_state_t* game_state) {
  struct level_editor_s* editor = &game_state->game_data.level_editor;
  if (rat_is_action_just_pressed_global("level_editor") && !editor->saving)
    boar_game_state_menu_init(game_state);

  // Converts mouse coordinates
  Uint32 mx, my;
  Uint32 ww, wh;
  Uint32 rw, rh;
  Uint32 rox, roy;
  float rsx, rsy;
  Uint32 mouse = SDL_GetMouseState(&mx,&my);
  SDL_GetWindowSize(game_state->window,&ww,&wh);
  SDL_RenderGetScale(game_state->renderer,&rsx,&rsy);
  rw = (float)GAME_WIDTH*rsx;
  rh = (float)GAME_HEIGHT*rsy;
  rox = ww/2 - rw/2;
  roy = wh/2 - rh/2;
  mx -= rox;
  my -= roy;
  mx = (float)mx / (float)rw * GAME_WIDTH;
  my = (float)my / (float)rh * GAME_HEIGHT;


  editor->selecting_tile = rat_is_action_pressed_global("tiles_palette");

  if (editor->selecting_tile) {
    int picker_x;
    int picker_y;
    picker_x = floor(mx/12.0);
    picker_x = MAX(MIN(picker_x,3),0);
    picker_y = floor(my/12.0);
    picker_y = MAX(MIN(picker_y,8),0);
    int tile_index = picker_y*4 + picker_x;
    if (mouse & SDL_BUTTON(1))
      editor->tile_to_paint = tile_index;
  }
  else {
    int tile_x;
    int tile_y;
    tile_x = floor(mx/16.0);
    tile_x = MAX(MIN(tile_x,15),0);
    tile_y = floor(my/16.0);
    tile_y = MAX(MIN(tile_y,14),0);
    editor->currently_selected_tile_x = tile_x;
    editor->currently_selected_tile_y = tile_y;
    if (mouse & SDL_BUTTON(1))
      editor->current_tilemap->tiles[tile_y][tile_x] = editor->tile_to_paint;
    if (mouse & SDL_BUTTON(3))
      editor->current_tilemap->tiles[tile_y][tile_x] = 0xFF;
  }

  if (rat_is_action_just_pressed_global("backspace"))
    if (strlen(editor->filename) > 0)
      editor->filename[strlen(editor->filename)-1] = 0;

  if (rat_is_action_just_pressed_global("save_level")) {
    if (!editor->saving) {
      editor->filename[0] = 0;
      SDL_StartTextInput();
    }
    else {
      SDL_StopTextInput();
      char filepath[128] = "levels/";
      strcat(filepath,editor->filename);
      strcat(filepath,".lvl");
      FILE* file = fopen(filepath,"wb");
      for (int i = 0; i < 15; i++)
        fwrite(editor->current_tilemap->tiles[i],sizeof(Uint8),16,file);
      fclose(file);
    }
    editor->saving = !editor->saving;
  }
}

bool
boar_game_state_update(boar_game_state_t* game_state) {
  Uint64 t = SDL_GetPerformanceCounter();
  Uint32 mouse_x = -1, mouse_y = -1;
  Uint64 delta = t-game_state->time.prev_frame;
  game_state->time.frame_time = (float)delta/SDL_GetPerformanceFrequency();
  game_state->time.delta_time = game_state->time.frame_time * game_state->time.time_scale;
  game_state->time.runtime += game_state->time.frame_time;
  game_state->time.prev_frame = t;

  rat_input_system_global_frame_update();

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        return true;
      case SDL_KEYDOWN:
        rat_input_system_global_update(e.key.keysym.scancode,true);
        break;
      case SDL_KEYUP:
        rat_input_system_global_update(e.key.keysym.scancode,false);
        break;
      case SDL_MOUSEMOTION:
        mouse_x = e.motion.x;
        mouse_y = e.motion.y;
        break;
      case SDL_TEXTINPUT:
        if (game_state->state == LEVEL_EDITOR && strlen(game_state->game_data.level_editor.filename) < 32)
          strcat(game_state->game_data.level_editor.filename,e.text.text);
        break;
    }
  }


  switch (game_state->state) {
    case LEVEL:
      boar_game_state_update_level(game_state);
      break;
    case LEVEL_EDITOR:
      boar_game_state_update_level_editor(game_state);
      break;
    case MENU:
      boar_game_state_update_menu(game_state);
      break;
    default:
      return false;
  }

  return false;
}

void
boar_game_state_draw_level(SDL_Renderer* renderer, struct level_s* level, atlas_t* font) {
  tilemap_tilemap_render(renderer,level->current_tilemap);
  player_draw(&level->player,renderer);
  if (level->player.level_finished)
    pidgeon_draw_text(renderer,font,"Space to go to the next level\nEscape to go back to the menu",8,GAME_HEIGHT-32);
}

void
boar_game_state_draw_menu(SDL_Renderer* renderer, boar_game_state_t* game_state, atlas_t* font) {
  struct menu_s* menu = &game_state->game_data.menu;
  SDL_Rect rect = {32, 4, 96*2, 128};
  SDL_RenderCopy(renderer,menu->titlescreen_texture,NULL,&rect);
  pidgeon_draw_text(renderer,font,"Left/Right to select level\nSpace to play level",8,GAME_HEIGHT-16);
  pidgeon_draw_text(renderer,font,game_state->current_level->level_name,128-64,GAME_HEIGHT-96);
}

void
boar_game_state_draw_level_editor(SDL_Renderer* renderer, struct level_editor_s* l_editor, atlas_t* font) {
  tilemap_tilemap_render(renderer,l_editor->current_tilemap);
  atlas_t* atlas = l_editor->current_tilemap->atlas;
  int x_div = atlas_atlas_divisions_x_get(atlas);
  int y_div = atlas_atlas_divisions_y_get(atlas);
  if (l_editor->saving) {
    SDL_Rect rect = {0,GAME_HEIGHT/2-8,GAME_WIDTH,8};
    SDL_SetRenderDrawColor(renderer,0x0F,0x0,0xF8,0x0F);
    SDL_RenderFillRect(renderer,&rect);
    SDL_SetRenderDrawColor(renderer,0x0F,0x0,0xF8,0xFF);
    SDL_RenderDrawRect(renderer,&rect);
    char* filename = l_editor->filename;
    printf("%s\n",filename);
    pidgeon_draw_text(renderer,font,filename,0,GAME_HEIGHT/2 - 8);
    return;
  }
  pidgeon_draw_text(renderer,font,"Tab to pick tiles\nEnter to save\n",0,GAME_HEIGHT-16);
  if (l_editor->selecting_tile) {
    for (int i = 0; i < x_div; i++) {
      for (int j = 0; j < y_div; j++) {
        atlas_atlas_render_ex(renderer, atlas,i*16,j*16,i,j,0,0.75,0.75,0,0);
      SDL_Rect rect = {l_editor->tile_to_paint%4 * 12, l_editor->tile_to_paint/4 *12,12,12};
      SDL_SetRenderDrawColor(renderer,0x0F,0x0,0xF8,0xFF);
      SDL_RenderDrawRect(renderer,&rect);
      }
    }
  }
  else {
    SDL_Rect rect = {l_editor->currently_selected_tile_x * 16, l_editor->currently_selected_tile_y*16,16,16};
    SDL_SetRenderDrawColor(renderer,0x0F,0x0,0xF8,0xFF);
    SDL_RenderDrawRect(renderer,&rect);
  }
}


void
boar_game_state_draw(boar_game_state_t* game_state) {
  SDL_SetRenderDrawColor(game_state->renderer,0xaa,0xff,0xff,0xff);
  SDL_RenderClear(game_state->renderer);
  switch (game_state->state) {
    case LEVEL:
      boar_game_state_draw_level(game_state->renderer,&game_state->game_data.level, game_state->font);
      break;
    case LEVEL_EDITOR:
      boar_game_state_draw_level_editor(game_state->renderer,&game_state->game_data.level_editor,game_state->font);
      break;
    case MENU:
      boar_game_state_draw_menu(game_state->renderer,game_state,game_state->font);
      break;
  }
  SDL_RenderPresent(game_state->renderer);
}
