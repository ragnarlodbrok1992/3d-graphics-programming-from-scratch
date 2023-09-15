#include "display.h"
#include "vector.h"
#include "utility.h"

SDL_Window*   WINDOW   = NULL;
SDL_Renderer* RENDERER = NULL;

// Forward declarations

// Engine variables
const char*   TITLE          = "3D Graphics Programming From Scratch.";
bool          ENGINE_RUNNING = false;

// Global buffers
uint32_t* COLOR_BUFFER            = NULL; 
SDL_Texture* COLOR_BUFFER_TEXTURE = NULL;

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

