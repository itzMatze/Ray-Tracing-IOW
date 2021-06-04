#ifndef RAY_TRACING_IOW_DIELECTRIC_H
#define RAY_TRACING_IOW_DIELECTRIC_H

#include "material.h"

float random_num();

class dielectric : public material
{
public:
    explicit dielectric(float ri) : ref_idx(ri)
    {}
    bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const override;
    float ref_idx;
};

#endif //RAY_TRACING_IOW_DIELECTRIC_H
