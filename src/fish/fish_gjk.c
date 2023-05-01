#include "../fish.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

typedef struct {
  float x;
  float y;
} fish_gjk_vec2_t;

static float
fish_gjk_vec2_dot(fish_gjk_vec2_t a, fish_gjk_vec2_t b) {
  return a.x*b.x + a.y*b.y;
}

static fish_gjk_vec2_t
fish_gjk_support_rect(fish_rect_t* rect, fish_gjk_vec2_t d) {
  fish_gjk_vec2_t p[4];
  fish_gjk_vec2_t best;
  p[0].x = -rect->w/2;
  p[1].x = rect->w/2;
  p[2].x = -rect->w/2;
  p[3].x = rect->w/2;

  p[0].y = -rect->h/2;
  p[1].y = rect->h/2;
  p[2].y = rect->h/2;
  p[3].y = -rect->h/2;
  float max_dot = -INFINITY;
  for (int i = 0; i < 4; i++) {
    fish_gjk_vec2_t c = p[i];
    float si = sin(rect->header.angle);
    float co = cos(rect->header.angle);
    float x, y;
    x = c.x;
    y = c.y;
    c.x = x*co - y*si;
    c.y = x*si + y*co;
    c.x += rect->header.x + (rect->w/2.0);
    c.y += rect->header.y + (rect->h/2.0);
    float dot = fish_gjk_vec2_dot(c,d);
    if (dot > max_dot) {
      max_dot = dot;
      best = c;
    }
  }
  return best;
}


static fish_gjk_vec2_t
fish_gjk_support_point(fish_point_t* point, fish_gjk_vec2_t d) {
  return (fish_gjk_vec2_t){point->header.x, point->header.y};
}

static fish_gjk_vec2_t
fish_gjk_support(fish_object_header_t* header, fish_gjk_vec2_t d) {
  switch (header->type) {
    case FISH_RECT:
      return fish_gjk_support_rect((fish_rect_t*)header,d);
      break;
  }
}

static fish_gjk_vec2_t
fish_gjk_support_diff(fish_object_header_t* s1, fish_object_header_t* s2, fish_gjk_vec2_t d) {
  fish_gjk_vec2_t v1, v2, nd;
  nd = (fish_gjk_vec2_t ){-d.x, -d.y};
  v1 = fish_gjk_support(s1,d);
  v2 = fish_gjk_support(s2,nd);
  return (fish_gjk_vec2_t){v1.x - v2.x, v1.y - v2.y};
}

static fish_gjk_vec2_t
fish_gjk_triple_cross(fish_gjk_vec2_t a, fish_gjk_vec2_t b, fish_gjk_vec2_t c) {
  float z = a.x*b.y-a.y*b.x;
  return (fish_gjk_vec2_t){-c.y * z, c.x * z};
}

bool
fish_gjk_intersects(fish_object_header_t* sh, fish_object_header_t* qh) {
  fish_gjk_vec2_t a, b, v, ab;
  a = fish_gjk_support_diff(sh,qh,(fish_gjk_vec2_t){1,1});
  v = (fish_gjk_vec2_t){-a.x,-a.y};
  b = fish_gjk_support_diff(sh,qh,v);

  if (fish_gjk_vec2_dot(b,v) <= 0)
    return false;
  
  ab.x = b.x-a.x;
  ab.y = b.y-a.y;

  v = fish_gjk_triple_cross(ab,(fish_gjk_vec2_t){-a.x,-a.y},ab);

  for (;;) {
    fish_gjk_vec2_t c, co, cb, ca, cap,cbp;
    c = fish_gjk_support_diff(sh,qh,v);

    if (fish_gjk_vec2_dot(c,v)<= 0)
      return false;

    co = (fish_gjk_vec2_t){-c.x,-c.y};

    cb = (fish_gjk_vec2_t){b.x-c.x, b.y-c.y};
    ca = (fish_gjk_vec2_t){a.x-c.x, a.y-c.y};

    cbp = fish_gjk_triple_cross(ca,cb,cb);
    cap = fish_gjk_triple_cross(cb,ca,ca);
  
    if (fish_gjk_vec2_dot(cap, co) > 0) {
      b = c;
      v = cap;
    } else if (fish_gjk_vec2_dot(cbp, co) > 0) {
      a = c;
      v = cbp;
    } else
      return true;
  }
}
