#include "display.h"
#include "mesh.h"
#include "array.h"

// triangle_t TRIANGLES_TO_RENDER[N_CUBE_FACES];
triangle_t* TRIANGLES_TO_RENDER = NULL;

vec3_t CAMERA_POSITION = { .x = 0.0f, .y = 0.0f, .z = -5.0f};
// vec3_t mesh.rotation = {.x = 0.0f, .y = 0.0f, .z = 0.0f};

// Projection variables
float FOV_FACTOR = 640;

// Engine variables
int PREVIOUS_FRAME_TIME = 0;

int WINDOW_WIDTH  = -1; // If it stays -1 - it's an error
int WINDOW_HEIGHT = -1;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // Query for monitor stuff
  // Fullscreen maximum width and height
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  WINDOW_WIDTH = display_mode.w;
  WINDOW_HEIGHT = display_mode.h;

  // First parameter can be NULL for borderless window TODO check this
  // Create a SDL window
  WINDOW = SDL_CreateWindow(
      NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_BORDERLESS
  );

  if (!WINDOW) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  // Create a SDL renderer
  RENDERER = SDL_CreateRenderer(
      WINDOW,
      -1,
      0
  );

  if (!RENDERER) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  // Changing video mode to fullscreen
  SDL_SetWindowFullscreen(WINDOW, SDL_WINDOW_FULLSCREEN);

  return true;
}

// Exercise 1 - draw grid
void render_grid(void) {
  uint32_t color_1 = 0xFFFF00FF; 
  uint32_t color_2 = 0xFFCCCC00;

  int grid_size = 32;

  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      if ((y % grid_size == 0) || (x % grid_size == 0)) COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color_2;
      else COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color_1;
    }
  }
}

// Exercise 2 - draw checkboard pattern
void render_checkboard_pattern(void) {
  uint32_t color_1 = 0xFFFF00FF; 
  uint32_t color_2 = 0xFFCCCC00;

  int tile_height = 64;
  int tile_width  = 32;

  int row_counter = 0;
  int column_counter = 0;
  uint32_t current_color = color_1;

  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      if (column_counter >= tile_width) {
        column_counter = 0;
        
        color_1 = current_color;
        current_color = color_2;
        color_2 = color_1;
      }

      set_pixel(x, y) = current_color;
      
      column_counter += 1;
    }

    if (row_counter >= tile_height) {
      row_counter = 0;

      color_1 = current_color;
      current_color = color_2;
      color_2 = color_1;
    }

    row_counter += 1;
  }
}

// Exercise 3 - draw rectangle
void render_rectangle(int top_left_row, int top_left_col, int size_row, int size_col, uint32_t color) {
  // This might not be necessary since we wont go into a loop if those are not correct
  // think about it
  if (size_row <= 0) return;
  if (size_col <= 0) return;
  if ((top_left_row <= 0) || (top_left_row > WINDOW_HEIGHT)) return;
  if ((top_left_col <= 0) || (top_left_col > WINDOW_WIDTH)) return;

  for (int y = top_left_col; y < top_left_col + size_col; y++) {
    for (int x = top_left_row; x < top_left_row + size_row; x++) {
      // set_pixel(x, y) = color;
      draw_pixel(x, y, color);
    }
  }
}

void render_color_buffer(void) {
  SDL_UpdateTexture(
      COLOR_BUFFER_TEXTURE,
      NULL,
      COLOR_BUFFER,
      (int)(WINDOW_WIDTH * sizeof(uint32_t))
  );
  SDL_RenderCopy(RENDERER, COLOR_BUFFER_TEXTURE, NULL, NULL);
}

void draw_pixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
    set_pixel(x, y) = color;
  }
}

// Exercise 4 - draw line (Bresenham's line algorithm)
void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
  // Bresenham's line algorithm
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  while (true) {
    draw_pixel(x0, y0, color);

    if (x0 == x1 && y0 == y1) break;

    int e2 = 2 * error;

    if (e2 >= dy) {
      if (x0 == x1) break;
      error += dy;
      x0 += sx;
    }

    if (e2 <= dx) {
      if (y0 == y1) break;
      error += dx;
      y0 += sy;
    }
  }
}

// Draw triangle function using draw_line
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
  draw_line(x0, y0, x1, y1, color);
  draw_line(x1, y1, x2, y2, color);
  draw_line(x2, y2, x0, y0, color);
}

void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color;
    }
  }
}

void teardown(void) {
  // Save some state

  // Free all required stuff
  free(COLOR_BUFFER);
  array_free(mesh.vertices);
  array_free(mesh.faces);

  // Some destroys for SDL
  SDL_DestroyRenderer(RENDERER);
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();
}

void setup(void) {
  // Allocating required bytes for COLOR_BUFFER
  COLOR_BUFFER = (uint32_t*) malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  COLOR_BUFFER_TEXTURE = SDL_CreateTexture(
      RENDERER,
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      WINDOW_WIDTH,
      WINDOW_HEIGHT
  );

  // Some tutorial setup
  // row 10 column 20 to color blue
  // This array is row first
  // pixel format = A R G B
  // COLOR_BUFFER[(WINDOW_WIDTH * 10) + 20] = 0xFF0000FF;

  // Initialize mesh for CUBE
  load_cube_mesh_data();
}

void update(void) {
  // // Naive loop
  // while (!SDL_TICKS_PASSED(SDL_GetTicks(), PREVIOUS_FRAME_TIME + FRAME_TARGET_TIME));

  // Calculating time to wait
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - PREVIOUS_FRAME_TIME);

  // Initialize the array of triangles to render
  TRIANGLES_TO_RENDER = NULL;

  // Only delay execution if we are running too fast
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }
  
  PREVIOUS_FRAME_TIME = SDL_GetTicks();

  mesh.rotation.x += 0.01f;
  mesh.rotation.y += 0.01f;
  mesh.rotation.z += 0.01f;

  // Loop all triangle faces
  int num_faces = array_length(mesh.faces);
  for (int i = 0; i < num_faces; i++ ) {
    face_t cube_face = mesh.faces[i];

    vec3_t face_vertices[3] = {
      mesh.vertices[cube_face.a - 1],
      mesh.vertices[cube_face.b - 1],
      mesh.vertices[cube_face.c - 1]
    };

    triangle_t projected_triangle;

    // Loop all three vertices of this current face and apply transformations
    for (int j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];

      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

      // Translate the vertex away from the camera
      transformed_vertex.z -= CAMERA_POSITION.z;

      // Project the current vertex/point
      vec2_t projected_point = project(transformed_vertex);

      // Scale and translate the projected point to the center of the screen
      projected_point.x += (WINDOW_WIDTH  / 2);
      projected_point.y += (WINDOW_HEIGHT / 2);

      projected_triangle.points[j] = projected_point;
    }

    // Save the projected triangle in the array of triangles to render
    // TRIANGLES_TO_RENDER[i] = projected_triangle;
    array_push(TRIANGLES_TO_RENDER, projected_triangle);

  }

  /*
  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = CUBE_POINTS[i];

    // Rotate
    vec3_t transformed_point = vec3_rotate_x(point, mesh.rotation.x);
           transformed_point = vec3_rotate_y(transformed_point, mesh.rotation.y);
           transformed_point = vec3_rotate_z(transformed_point, mesh.rotation.z);

    // Translate the points away from the camera
    transformed_point.z -= CAMERA_POSITION.z;

    // Project the current point
    vec2_t projected_point = project(transformed_point);

    PROJECTED_POINTS[i] = projected_point;
  }
  */
}

void render(void) {
  // Loop all projected triangles and render them
  // for (int i = 0; i < N_CUBE_FACES; i++) {

  int render_size = array_length(TRIANGLES_TO_RENDER);

  for (int i = 0; i < render_size; i++) {
    // vec2_t projected_point = PROJECTED_POINTS[i];

    // print_vec2(projected_point);
    triangle_t triangle = TRIANGLES_TO_RENDER[i];

    // Draw vertex points
#pragma warning(push)
#pragma warning(disable : 4244)
    render_rectangle(triangle.points[0].x,
        triangle.points[0].y,
        3,
        3,
        0xFFFFFF00);

    render_rectangle(triangle.points[1].x,
        triangle.points[1].y,
        3,
        3,
        0xFFFFFF00);

    render_rectangle(triangle.points[2].x,
        triangle.points[2].y,
        3,
        3,
        0xFFFFFF00);

    // Draw triangle
    draw_triangle(triangle.points[0].x,
        triangle.points[0].y,
        triangle.points[1].x,
        triangle.points[1].y,
        triangle.points[2].x,
        triangle.points[2].y,
        0xFF00FF00);
#pragma warning(pop)

  }

  // Clear the array of triangles to render every frame loop
  array_free(TRIANGLES_TO_RENDER);

  // TEST - render line
  /*
  draw_line(100, 100, 100, 200, 0xFF00FF00);
  draw_line(100, 200, 200, 200, 0xFF00FF00);
  draw_line(200, 100, 200, 200, 0xFF00FF00);
  draw_line(100, 100, 200, 100, 0xFF00FF00);
  */

  // Final render of the collor buffer
  render_color_buffer();
  clear_color_buffer(0xFF000000);

  // Last render call
  SDL_RenderPresent(RENDERER);
}

// Function receiving 3D vector and returns projected 2D point
vec2_t project(vec3_t point) {
  // Ortographic projection
  vec2_t projected_point = {
    .x = (FOV_FACTOR * point.x) / point.z,
    .y = (FOV_FACTOR * point.y) / point.z
  };

  return projected_point;
}
