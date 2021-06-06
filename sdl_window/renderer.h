#ifndef RAY_TRACER_CPP_RENDERER_H
#define RAY_TRACER_CPP_RENDERER_H

#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cassert>

#include "color.h"

class renderer
{
public:
    renderer(int width, int render_width, int render_height);
    ~renderer();
    void clean_up_sdl() const;
    void render_frame();
    inline void set_pixel(int x, int y, color color);
    const void* get_pixels();

    int render_width;
private:
    SDL_Window* win = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;
    SDL_Surface* bitmapSurface = nullptr;
};

#endif //RAY_TRACER_CPP_RENDERER_H
