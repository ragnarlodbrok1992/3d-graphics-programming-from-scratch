#ifndef _H_TRIANGLE
#define _H_TRIANGLE

#include "vector.h"

typedef struct {
  int a;
  int b;
  int c;
} face_t;

typedef struct {
  vec2_t points[3];
} triangle_t;

#endif // _H_TRIANGLE

