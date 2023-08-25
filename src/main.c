// This has to be in Windows code for SDL
// TODO check this on Linux
#define SDL_MAIN_HANDLED

// Local macros
#define set_pixel(a, b) COLOR_BUFFER[(WINDOW_WIDTH * b) + a]

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

// Forward declarations
void render_grid(void);
void render_checkboard_pattern(void);
void render_rectangle(int top_left_row, int top_left_col, int size_row, int size_col, uint32_t color);

// Engine variables
const char*   TITLE          = "3D Graphics Programming From Scratch.";
SDL_Window*   WINDOW         = NULL;
SDL_Renderer* RENDERER       = NULL;
bool          ENGINE_RUNNING = false;

int           WINDOW_WIDTH;
int           WINDOW_HEIGHT;

// Global buffers
uint32_t* COLOR_BUFFER            = NULL; 
SDL_Texture* COLOR_BUFFER_TEXTURE = NULL;

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

void update(void) {
  // TODO:
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

void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color;
    }
  }
}

void render(void) {
  SDL_SetRenderDrawColor(RENDERER, 0xff, 0x00, 0x00, 0xff);
  SDL_RenderClear(RENDERER);

  clear_color_buffer(0xFFFFFF00);

  // Update color buffer
  // render_grid();
  render_checkboard_pattern();
  render_rectangle(200, 200, 100, 100, 0xFF00FF00);

  // Final render of the collor buffer
  render_color_buffer();

  // Last render call
  SDL_RenderPresent(RENDERER);
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
  if (size_row <= 0) return;
  if (size_col <= 0) return;
  if ((top_left_row <= 0) || (top_left_row > WINDOW_HEIGHT)) return;
  if ((top_left_col <= 0) || (top_left_col > WINDOW_WIDTH)) return;

  for (int y = top_left_col; y < top_left_col + size_col; y++) {
    for (int x = top_left_row; x < top_left_row + size_row; x++) {
      set_pixel(x, y) = color;
    }
  }
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

  while (ENGINE_RUNNING) {
    process_input();
    update();
    render();
  }

  // Last message - so everything went well

  teardown();

  printf("Goodbye.\n");
  return 0;
}

