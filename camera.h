#ifndef RAY_TRACING_IOW_CAMERA_H
#define RAY_TRACING_IOW_CAMERA_H

#include "ray.h"

class camera
{
public:
    camera(int nx, int ny)
    {
        origin = glm::vec3();
        lower_left_corner = glm::vec3(-(float(nx) / float(ny)), -1.0f, -1.0f);
        horizontal = glm::vec3(2 * (float(nx) / float(ny)), 0.0f, 0.0f);
        vertical = glm::vec3(0.0f, 2.0f, 0.0f);
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
