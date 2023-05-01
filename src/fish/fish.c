#include "../fish.h"

void
fish_physics_object_step(fish_object_header_t* header, float delta_time) {
  header->velocity_x += (header->force_x / header->mass) * delta_time;
  header->velocity_y += (header->force_y / header->mass) * delta_time;

  header->x += header->velocity_x * delta_time;
  header->y += header->velocity_y * delta_time;

  header->force_x = 0;
  header->force_y = 0;
}

void
fish_physics_world_step(fish_physics_world_t* world, float delta_time) {
  // Dynamics step
  for (int i = 0; i < FISH_MAX_OBJECTS_PER_TYPE; i++) {
    if (world->point_objects[i].header.enabled) 
      fish_physics_object_step((fish_object_header_t*)&world->point_objects[i],delta_time);
    if (world->line_objects[i].header.enabled) 
      fish_physics_object_step((fish_object_header_t*)&world->line_objects[i],delta_time);
    if (world->rect_objects[i].header.enabled) 
      fish_physics_object_step((fish_object_header_t*)&world->rect_objects[i],delta_time);
    if (world->polygon_objects[i].header.enabled) 
      fish_physics_object_step((fish_object_header_t*)&world->polygon_objects[i],delta_time);
    if (world->circle_objects[i].header.enabled) 
      fish_physics_object_step((fish_object_header_t*)&world->circle_objects[i],delta_time);
    if (world->capsule_objects[i].header.enabled) 
      fish_physics_object_step((fish_object_header_t*)&world->capsule_objects[i],delta_time);
    if (world->convex_hull_objects[i].header.enabled) 
      fish_physics_object_step((fish_object_header_t*)&world->convex_hull_objects[i],delta_time);
  }
}
