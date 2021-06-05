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
    renderer(int width, int height);
    ~renderer();
    void clean_up_sdl() const;
    void render_frame();
    inline void set_pixel(int x, int y, color color);
    inline void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
    {
        //assert(x < width && y < height);
        //SDL_LockSurface(bitmapSurface);
        unsigned int color = r;
        color <<= 8;
        color += g;
        color <<= 8;
        color += b;
        color <<= 8;
        color += 255;
        ((unsigned int*) (bitmapSurface->pixels))[(x + width * y)] = color;
        //SDL_UnlockSurface(bitmapSurface);
    }

    int width, height;
private:
    SDL_Window* win = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;
    SDL_Surface* bitmapSurface = nullptr;
};

#endif //RAY_TRACER_CPP_RENDERER_H
