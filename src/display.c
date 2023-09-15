#include "display.h"

// Static values
#define N_POINTS (9 * 9 * 9)
vec3_t CUBE_POINTS[N_POINTS]; // from -1 to 1 (9x9x9) cube
vec2_t PROJECTED_POINTS[N_POINTS];

vec3_t CAMERA_POSITION = { .x = 0.0f, .y = 0.0f, .z = -5.0f};
vec3_t CUBE_ROTATION = {.x = 0.0f, .y = 0.0f, .z = 0.0f};

// Projection variables
float FOV_FACTOR = 640;

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

  // Some destroys
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

void update(void) {



  CUBE_ROTATION.x += 0.01f;
  CUBE_ROTATION.y += 0.01f;
  CUBE_ROTATION.z += 0.01f;

  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = CUBE_POINTS[i];

    // Rotate
    vec3_t transformed_point = vec3_rotate_x(point, CUBE_ROTATION.x);
           transformed_point = vec3_rotate_y(transformed_point, CUBE_ROTATION.y);
           transformed_point = vec3_rotate_z(transformed_point, CUBE_ROTATION.z);

    // Translate the points away from the camera
    transformed_point.z -= CAMERA_POSITION.z;

    // Project the current point
    vec2_t projected_point = project(transformed_point);

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

// Function receiving 3D vector and returns projected 2D point
vec2_t project(vec3_t point) {
  // Ortographic projection
  vec2_t projected_point = {
    .x = (FOV_FACTOR * point.x) / point.z,
    .y = (FOV_FACTOR * point.y) / point.z
  };

  return projected_point;
}
