#ifndef FISH_H
#include <stdint.h>
#include <stdbool.h>
#define FISH_H
#define FISH_MAX_OBJECTS_PER_TYPE 256
#define FISH_MAX_POLYGON_VERTS 128

typedef enum {
  FISH_POINT,
  FISH_LINE,
  FISH_RECT,
  FISH_POLYGON,
  FISH_CIRCLE,
  FISH_CAPSULE,
  FISH_CONVEX_HULL,
  FISH_SHAPE_COUNT,
} FISH_SHAPE;

typedef struct fish_object_header_s {
  uint32_t layer;
  uint32_t mask;
  float x;
  float y;
  float angle;
  float mass;
  float velocity_x;
  float velocity_y;
  float force_x;
  float force_y;
  FISH_SHAPE type;
  uint8_t enabled;
} fish_object_header_t;

typedef struct fish_rect_s {
  fish_object_header_t header;
  float w;
  float h;
} fish_rect_t;

typedef struct fish_circle_s {
  fish_object_header_t header;
  float r;
} fish_circle_t;

typedef struct fish_line_s {
  fish_object_header_t header;
  float x2;
  float y2;
} fish_line_t;

typedef struct fish_point_s {
  fish_object_header_t header;
} fish_point_t;

typedef struct fish_polygon_s {
  fish_object_header_t header;
  float x_verts[FISH_MAX_POLYGON_VERTS];
  float y_verts[FISH_MAX_POLYGON_VERTS];
} fish_polygon_t;

typedef struct fish_capsule_s {
  fish_object_header_t header;
  float x2;
  float y2;
  float r;
} fish_capsule_t;

typedef struct fish_convex_hull_s {
  fish_object_header_t header;
  float x_verts[FISH_MAX_POLYGON_VERTS];
  float y_verts[FISH_MAX_POLYGON_VERTS];
} fish_convex_hull_t;

typedef struct fish_collision_data_s {
  float point_x;
  float point_y;
  float normal_x;
  float normal_y;
} fish_collision_data_t;


typedef struct fish_physics_world_s {
  fish_point_t point_objects[FISH_MAX_OBJECTS_PER_TYPE];
  fish_line_t line_objects[FISH_MAX_OBJECTS_PER_TYPE];
  fish_rect_t rect_objects[FISH_MAX_OBJECTS_PER_TYPE];
  fish_polygon_t polygon_objects[FISH_MAX_OBJECTS_PER_TYPE];
  fish_circle_t circle_objects[FISH_MAX_OBJECTS_PER_TYPE];
  fish_capsule_t capsule_objects[FISH_MAX_OBJECTS_PER_TYPE];
  fish_convex_hull_t convex_hull_objects[FISH_MAX_OBJECTS_PER_TYPE];
} fish_physics_world_t;

void
fish_physics_object_step(fish_object_header_t* header, float delta_time);

void
fish_physics_world_step(fish_physics_world_t* world, float delta_time);

#endif
