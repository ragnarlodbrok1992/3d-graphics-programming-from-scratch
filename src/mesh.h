#ifndef _H_MESH
#define _H_MESH

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
extern vec3_t mesh_vertices[N_MESH_VERTICES];

#define N_MESH_FACES (6 * 2)
extern face_t mesh_faces[N_MESH_FACES];

#endif // _H_MESH
