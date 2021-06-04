#ifndef RAY_TRACING_IOW_LAMBERTIAN_H
#define RAY_TRACING_IOW_LAMBERTIAN_H

#include "material.h"

class lambertian : public material
{
public:
    explicit lambertian(const glm::vec3 a) : albedo(a)
    {}
    bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const override;
    glm::vec3 albedo;
};

#endif //RAY_TRACING_IOW_LAMBERTIAN_H
