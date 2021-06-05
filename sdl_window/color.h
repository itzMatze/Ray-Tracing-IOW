#ifndef RAY_TRACER_CPP_COLOR_H
#define RAY_TRACER_CPP_COLOR_H

class color
{
public:
    color();
    color(float r, float g, float b);
    color(float r, float g, float b, float a);
    unsigned int get_hex_color() const;
private:
    float r, g, b, a;
};

#endif //RAY_TRACER_CPP_COLOR_H
