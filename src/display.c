#include "display.h"

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
      set_pixel(x, y) = color;
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
  if (x < WINDOW_WIDTH && y < WINDOW_HEIGHT) {
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
