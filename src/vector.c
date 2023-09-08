#include "vector.h"

#include <math.h>

vec3_t vec3_rotate_x(vec3_t v, float angle) {
  vec3_t rotated_vector = {
    .x = v.x,
    .y = v.y * (float) cos(angle) - v.z * (float) sin(angle),
    .z = v.y * (float) sin(angle) + v.z * (float) cos(angle)
  };
  return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
  vec3_t rotated_vector = {
    .x = v.x * (float) cos(angle) - v.z * (float) sin(angle),
    .y = v.y,
    .z = v.x * (float) sin(angle) + v.z * (float) cos(angle)
  };
  return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
  vec3_t rotated_vector = {
    .x = v.x * (float) cos(angle) - v.y * (float) sin(angle),
    .y = v.x * (float) sin(angle) + v.y * (float) cos(angle),
    .z = v.z
  };
  return rotated_vector;
}

// TODO: implementation of all vector functions

