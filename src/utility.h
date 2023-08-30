#ifndef _H_UTILITY
#define _H_UTILITY

#include "vector.h"
#include <stdio.h>

void print_vec2(vec2_t printed_point) {
  printf("Vec2_t: (%f, %f)\n",
      printed_point.x,
      printed_point.y);
}

void print_vec3(vec3_t printed_point) {
  printf("Vec3_t: (%f, %f, %f)\n",
      printed_point.x,
      printed_point.y,
      printed_point.z);
}

#endif /* _H_UTILITY */
