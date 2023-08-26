// This has to be in Windows code for SDL
// TODO check this on Linux

#include "display.h"

SDL_Window*   WINDOW = NULL;
SDL_Renderer* RENDERER = NULL;

// Forward declarations

// Engine variables
const char*   TITLE          = "3D Graphics Programming From Scratch.";
bool          ENGINE_RUNNING = false;

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

