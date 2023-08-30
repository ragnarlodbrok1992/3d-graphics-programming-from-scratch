#ifndef _H_DISPLAY
#define _H_DISPLAY

#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
  #include <SDL.h>
#endif

#ifdef  __linux__
  #include <SDL2/SDL.h>
#endif


#define set_pixel(a, b) COLOR_BUFFER[(WINDOW_WIDTH * b) + a]

extern SDL_Window*   WINDOW;
extern SDL_Renderer* RENDERER;
extern int           WINDOW_WIDTH;
extern int           WINDOW_HEIGHT;
extern uint32_t*     COLOR_BUFFER; 
extern SDL_Texture*  COLOR_BUFFER_TEXTURE;

bool initialize_window(void);
void render_grid(void);
void render_checkboard_pattern(void);
void render_rectangle(int top_left_row, int top_left_col, int size_row, int size_col, uint32_t color);
void render_color_buffer(void);
void draw_pixel(int x, int y, uint32_t color);
void clear_color_buffer(uint32_t color);
void teardown(void);

#endif /* _H_DISPLAY */

