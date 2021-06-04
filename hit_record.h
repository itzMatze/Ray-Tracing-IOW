#ifndef RAY_TRACING_IOW_HIT_RECORD_H
#define RAY_TRACING_IOW_HIT_RECORD_H

class material;

struct hit_record
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    material* mat;
};

#endif //RAY_TRACING_IOW_HIT_RECORD_H
