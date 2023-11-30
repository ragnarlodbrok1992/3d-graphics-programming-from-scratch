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


#include "vector.h"

#define set_pixel(a, b) COLOR_BUFFER[(WINDOW_WIDTH * b) + a]

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

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
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void clear_color_buffer(uint32_t color);
void teardown(void);

void setup(void);
void update(void);
void render(void);

vec2_t project(vec3_t point);

#endif /* _H_DISPLAY */

