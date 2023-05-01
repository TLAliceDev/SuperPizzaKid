#include <math.h>
#include "../player.h"
#include "../rat.h"
#include "../owl.h"
#include "../atlas.h"
#include "../tilemap.h"
#include "../raccoon.h"

enum {
  IDLE,
  WALK,
  JUMP,
  FALL,
  DIE,
  FINISHED,
} PLAYER_ANIMATIONS;

void player_die(player_t* player) {
  Mix_PlayChannel(-1,player->sfxs.die_sfx,0);
  player->dead = true;
}

void player_respawn(player_t* player) {
  player->vx = 0;
  player->vy = 0;
  player->x = player->spawn_x;
  player->y = player->spawn_y;
  player->flip = false;
  player->dead = false;
  player->dead_timer = 0.0;
}

void
player_update(player_t* player, tilemap_t* tilemap, float delta_time) {
  const float PLAYER_SPEED = 160.0;
  const float GRAVITY = 245.0;
  const float JUMP_FORCE = 164.0;
  if (player->dead) {
    player->current_animation = DIE;
    player->dead_timer += delta_time;
    player->y -= sin(player->dead_timer*10.0 + M_PI/2.5) * 3.0 * (player->dead_timer*player->dead_timer*16.0);
    if (player->dead_timer > 0.5)
      player_respawn(player);
    return;
  }
  if (player->level_finished) {
    player->current_animation = FINISHED;
    return;
  }
  int x_axis = rat_is_action_pressed_global("right") - rat_is_action_pressed_global("left");
  if (x_axis != 0) {
    player->current_animation = WALK;
    if (x_axis == -1)
      player->flip = 1;
    else
      player->flip = 0;
  } else {
    player->current_animation = IDLE;
  }
  if (player->vy < 0)
    player->current_animation = JUMP;
  else if (player->vy > 0)
    player->current_animation = FALL;


  if (player->prev_animation != player->current_animation) {
    player->animation_timer = 0.0;
  }
  
  player->prev_animation = player->current_animation;
  player->animation_timer += delta_time;

  player->jump_buffer -= delta_time;
  player->ground_buffer -= delta_time;


  player->vx = lerp(player->vx,x_axis*PLAYER_SPEED,delta_time*10.0);
  player->vy += GRAVITY * delta_time;
  for (int i = 0; i < 16; i++ ){
    float old_x = player->x;
    int intersection_x, intersection_y;
    player->x += (player->vx*delta_time / 16.0);

    int collided = tilemap_tilemap_intersects(tilemap,player->x+4,player->y+4,6,11,&intersection_x,&intersection_y) ;
    if (collided != 0xFF) {
      switch (collided) {
        case 12:
        case 14:
        case 30:
          player_die(player);
          break;
        case 28:
          Mix_PlayChannel(-1,player->sfxs.coin_sfx,0);
          tilemap_tilemap_set_tile(tilemap,intersection_x,intersection_y,0xFF);
          break;
        case 32:
          Mix_PlayChannel(-1,player->sfxs.win_sfx,0);
          player->level_finished = true;
          break;
        default:
          player->vx = 0.0;
          player->x = old_x;
          break;
      }
    }

    float old_y = player->y;
    player->y += (player->vy*delta_time / 16.0);

    collided = tilemap_tilemap_intersects(tilemap,player->x+4,player->y+4,6,11,&intersection_x,&intersection_y) ;
    if (collided != 0xFF) {
      switch (collided) {
        case 12:
        case 13:
        case 29:
          player_die(player);
          break;
        case 28:
          Mix_PlayChannel(-1,player->sfxs.coin_sfx,0);
          tilemap_tilemap_set_tile(tilemap,intersection_x,intersection_y,0xFF);
          break;
        case 32:
          Mix_PlayChannel(-1,player->sfxs.win_sfx,0);
          player->level_finished = true;
          break;
        default:
          if (player->vy > 0)
            player->ground_buffer = 0.15;
          player->vy = 0.0;
          player->y = old_y;
          break;
      }
    }

    if (player->y >= 240)
      player_die(player);
  }

  if (rat_is_action_just_pressed_global("jump"))
    player->jump_buffer = 0.15;


  if (player->jump_buffer > 0.0 && player->ground_buffer > 0.0) {
    Mix_PlayChannel(-1,player->sfxs.jump_sfx,0);
    player->jump_buffer = 0.0;
    player->ground_buffer = 0.0;
    player->vy = -JUMP_FORCE;
  }

  if (player->step_timer > 0)
    player->step_timer -= delta_time;

  if (player->current_animation == WALK) {
    if (player->step_timer <= 0) {
      Mix_PlayChannel(-1,player->sfxs.step_sfx,0);
      player->step_timer = 0.2;
    }
  }

  if (rat_is_action_just_released_global("jump") && player->vy < 0)
    player->vy *= 0.5;
}

void
player_draw(player_t* player, SDL_Renderer* renderer) {
  int anim_frame = floor(player->animation_timer*6);
  switch (player->current_animation) {
    case JUMP:
    case IDLE:
    case DIE:
    case FINISHED:
      anim_frame = 0;
      break;
    case WALK:
    case FALL:
      anim_frame %= 2;
      break;
  }
  atlas_atlas_render_ex(renderer, player->texture,
                        player->x, player->y,
                        anim_frame,
                        player->current_animation,
                        0,1,1,
                        player->flip,0);
}
