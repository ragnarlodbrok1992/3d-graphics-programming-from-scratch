#ifndef _H_MESH
#define _H_MESH

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
  vec3_t rotation;
  vec3_t* vertices; // dynamic array of vertices
  face_t* faces;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data();

#endif // _H_MESH
