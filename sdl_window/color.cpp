#include "color.h"

color::color() : r(0), g(0), b(0), a(0)
{
}

color::color(float r, float g, float b) : r(r), g(g), b(b), a(1)
{
}

color::color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{
}

unsigned int color::get_hex_color() const
{
    unsigned int color = r > 1.0f ? 255 : static_cast<unsigned int>(r * 255);
    color <<= 8;
    color += g > 1.0f ? 255 : static_cast<unsigned int>(g * 255);
    color <<= 8;
    color += b > 1.0f ? 255 : static_cast<unsigned int>(b * 255);
    color <<= 8;
    color += a > 1.0f ? 255 : static_cast<unsigned int>(a * 255);
    return color;
}
