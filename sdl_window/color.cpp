#include "color.h"

color::color() : values(glm::vec4(0.0f))
{
}

color::color(float r, float g, float b) : values(glm::vec4(r, g, b, 1.0f))
{
}

color::color(float r, float g, float b, float a) : values(glm::vec4(r, g, b, a))
{
}

unsigned int color::get_hex_color() const
{
    unsigned int color = values.a > 1.0f ? 255 : static_cast<unsigned int>(values.a * 255);
    color <<= 8;
    color += values.b > 1.0f ? 255 : static_cast<unsigned int>(values.b * 255);
    color <<= 8;
    color += values.g > 1.0f ? 255 : static_cast<unsigned int>(values.g * 255);
    color <<= 8;
    color += values.r > 1.0f ? 255 : static_cast<unsigned int>(values.r * 255);
    return color;
}

