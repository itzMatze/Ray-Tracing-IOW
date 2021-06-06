#include "metal.h"

bool metal::scatter(const ray& r_in, const hit_record& rec, glm::vec4& attenuation, ray& scattered) const
{
    glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = glm::vec4(albedo, 1.0f);
    return (glm::dot(scattered.direction, rec.normal) > 0);
}
