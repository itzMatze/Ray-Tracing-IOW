#ifndef RAY_TRACING_IOW_HITABLE_LIST_H
#define RAY_TRACING_IOW_HITABLE_LIST_H

#include "hitable.h"

class hitable_list : public hitable
{
public:
    hitable_list() = default;
    hitable_list(hitable** list, int n) : list(list), list_size(n)
    {
    }
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    hitable** list;
    int list_size;
};

#endif //RAY_TRACING_IOW_HITABLE_LIST_H
