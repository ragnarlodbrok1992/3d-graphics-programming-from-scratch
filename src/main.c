// This has to be in Windows code for SDL
// TODO check this on Linux
#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

const char*   TITLE          = "3D Graphics Programming From Scratch.";
SDL_Window*   WINDOW         = NULL;
SDL_Renderer* RENDERER       = NULL;
bool          ENGINE_RUNNING = false;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // First parameter can be NULL for borderless window TODO check this
  // Create a SDL window
  WINDOW = SDL_CreateWindow(
      NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800,
      600,
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

  return true;
}

void setup(void) {
  // TODO:
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

  // ...

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
  printf("Goodbye.\n");
  return 0;
}

