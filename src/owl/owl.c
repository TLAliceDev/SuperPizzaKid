#include "../owl.h"

float
lerp(float a, float b, float t) {
  return (1.0-t)*a + t*b;
}

