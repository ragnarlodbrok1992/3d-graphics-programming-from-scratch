#include "display.h"
#include "vector.h"
#include "utility.h"

SDL_Window*   WINDOW   = NULL;
SDL_Renderer* RENDERER = NULL;

// Forward declarations

// Engine variables
const char*   TITLE          = "3D Graphics Programming From Scratch.";
bool          ENGINE_RUNNING = false;

// Static values
#define N_POINTS (9 * 9 * 9)
vec3_t CUBE_POINTS[N_POINTS]; // from -1 to 1 (9x9x9) cube
vec2_t PROJECTED_POINTS[N_POINTS];                              

// Projection variables
float FOV_FACTOR = 128;

// Global buffers
uint32_t* COLOR_BUFFER            = NULL; 
SDL_Texture* COLOR_BUFFER_TEXTURE = NULL;

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

  // Setting up cube of points
  int point_select = 0;
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float  z = -1; z <= 1; z += 0.25) {
        vec3_t new_point = { .x = x, .y = y, .z = z };
        CUBE_POINTS[point_select++] = new_point;
      }
    }
  }
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      ENGINE_RUNNING = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) ENGINE_RUNNING = false;
      break;
  }
}

// Function receiving 3D vector and returns projected 2D point
vec2_t project(vec3_t point) {
  // Ortographic projection
  vec2_t projected_point = {
    .x = (FOV_FACTOR * point.x),
    .y = (FOV_FACTOR * point.y)
  };

  return projected_point;
}

void update(void) {
  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = CUBE_POINTS[i];

    // Project the current point
    vec2_t projected_point = project(point);

    PROJECTED_POINTS[i] = projected_point;
  }
}

void render(void) {
  // Loop all projected points and render them
  for (int i = 0; i < N_POINTS; i++) {
    vec2_t projected_point = PROJECTED_POINTS[i];

    // print_vec2(projected_point);

    render_rectangle(
        (int) projected_point.x + (WINDOW_WIDTH  / 2),
        (int) projected_point.y + (WINDOW_HEIGHT / 2),
        4,
        4,
        0xFFFFFF00
        );
  }

  // Final render of the collor buffer
  render_color_buffer();
  clear_color_buffer(0xFF000000);

  // Last render call
  SDL_RenderPresent(RENDERER);
}


int main(int argc, char* argv[])
{
  // Remove warning about unused variables
  // TODO: parse config file here in the future
  (void)argc;
  (void)argv;

  ENGINE_RUNNING = initialize_window();

  // Main engine loop
  setup();

  // vec2_t vector_hector = {1.0, 2.0};
  // (void) vector_hector;

  while (ENGINE_RUNNING) {
    process_input();
    update();
    render();
  }

  teardown();

  // Last message - so everything went well
  printf("Goodbye.\n");
  return 0;
}

