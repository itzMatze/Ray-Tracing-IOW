#ifndef RAY_TRACING_IOW_MATERIAL_H
#define RAY_TRACING_IOW_MATERIAL_H

#include "glm.hpp"
#include "hit_record.h"
#include "ray.h"

glm::vec3 random_in_unit_sphere();

class material
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec4& attenuation, ray& scattered) const = 0;
};

#endif //RAY_TRACING_IOW_MATERIAL_H
