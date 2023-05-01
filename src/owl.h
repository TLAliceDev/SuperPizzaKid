#ifndef OWL_H
#define OWL_H

#define MIN(x,y) (x) > (y) ? (y) : (x)
#define MAX(x,y) (x) < (y) ? (y) : (x)
#define SIGN(x) (x) < 0 ? -1 : 1

float
lerp(float a, float b, float t);

#endif
