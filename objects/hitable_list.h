#ifndef RAY_TRACING_IOW_HITABLE_LIST_H
#define RAY_TRACING_IOW_HITABLE_LIST_H

#include <utility>
#include <vector>

#include "hitable.h"

class hitable_list : public hitable
{
public:
    hitable_list() = default;
    explicit hitable_list(std::vector<hitable*>& objects) : objects(objects)
    {
    }
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    std::vector<hitable*>& objects;
};

#endif //RAY_TRACING_IOW_HITABLE_LIST_H
