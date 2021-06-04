
#ifndef RAY_TRACING_IOW_HITABLE_H
#define RAY_TRACING_IOW_HITABLE_H

#include "ray.h"
#include "hit_record.h"

class material;

class hitable
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif //RAY_TRACING_IOW_HITABLE_H
