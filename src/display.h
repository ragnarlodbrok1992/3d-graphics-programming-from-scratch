#ifndef _H_DISPLAY
#define _H_DISPLAY

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

SDL_Window*   WINDOW;
SDL_Renderer* RENDERER;
int           WINDOW_WIDTH;
int           WINDOW_HEIGHT;

bool initialize_window(void);
void render_grid(void);
void render_checkboard_pattern(void);
void render_rectangle(int top_left_row, int top_left_col, int size_row, int size_col, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void teardown(void);

#endif /* _H_DISPLAY */

