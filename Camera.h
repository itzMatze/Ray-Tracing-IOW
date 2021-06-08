#ifndef RAY_TRACING_IOW_CAMERA_H
#define RAY_TRACING_IOW_CAMERA_H

#include "Ray.h"

class Camera
{
public:
    Camera(glm::vec3 origin, glm::vec3 look_at, glm::vec3 up, float vfov, float aspect, float aperture, float focus_dist) : origin(origin)
    {
        lens_radius = aperture / 2.0f;
        float theta = vfov * std::_Pi / 180.0f;
        float half_height = tan(theta / 2.0f);
        float half_width = aspect * half_height;
        w = glm::normalize(origin - look_at);
        u = glm::normalize(glm::cross(up, w));
        v = glm::cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2.0f * half_width * focus_dist * u;
        vertical = 2.0f * half_height * focus_dist * v;
    }
    Ray get_ray(float s, float t) const
    {
        glm::vec3 rd = lens_radius * random_in_unit_sphere();
        rd.z = 0.0f;
        glm::vec3 offset = u * rd.x + v * rd.y;
        return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }
    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u, v, w;
    float lens_radius;
};

#endif //RAY_TRACING_IOW_CAMERA_H
