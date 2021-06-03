#ifndef RAY_TRACING_IOW_SPHERE_H
#define RAY_TRACING_IOW_SPHERE_H

#include "hitable.h"

class sphere : public hitable
{
public:
    sphere() = default;
    sphere(glm::vec3 center, float radius) : center(center), radius(radius)
    {}
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    glm::vec3 center;
    float radius;
};

#endif //RAY_TRACING_IOW_SPHERE_H
