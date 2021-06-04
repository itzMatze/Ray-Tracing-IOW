#ifndef RAY_TRACING_IOW_CAMERA_H
#define RAY_TRACING_IOW_CAMERA_H

#include "ray.h"

class camera
{
public:
    camera(glm::vec3 origin, glm::vec3 look_at, glm::vec3 up, float vfov, float aspect) : origin(origin)
    {
        glm::vec3 u, v, w;
        float theta = vfov * std::_Pi / 180.0f;
        float half_height = tan(theta / 2.0f);
        float half_width = aspect * half_height;
        w = glm::normalize(origin - look_at);
        u = glm::normalize(glm::cross(up, w));
        v = glm::cross(w, u);
        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2.0f * half_width * u;
        vertical = 2.0f * half_height * v;
    }
    ray get_ray(float u, float v) const
    {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};

#endif //RAY_TRACING_IOW_CAMERA_H
