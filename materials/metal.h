#ifndef RAY_TRACING_IOW_METAL_H
#define RAY_TRACING_IOW_METAL_H

#include "material.h"

class metal : public material
{
public:
    metal(const glm::vec3 a, float fuzz) : albedo(a)
    {this->fuzz = fuzz < 1 ? fuzz : 1.0f;}

    bool scatter(const ray& r_in, const hit_record& rec, glm::vec4& attenuation, ray& scattered) const override;

    glm::vec3 albedo;
    float fuzz;
};

#endif //RAY_TRACING_IOW_METAL_H
