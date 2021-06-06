#ifndef RAY_TRACER_CPP_COLOR_H
#define RAY_TRACER_CPP_COLOR_H

#include <vec4.hpp>

class color
{
public:
    color();
    color(float r, float g, float b);
    color(float r, float g, float b, float a);
    unsigned int get_hex_color() const;

    glm::vec4 values;
};

#endif //RAY_TRACER_CPP_COLOR_H
