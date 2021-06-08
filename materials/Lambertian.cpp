#include "Lambertian.h"

bool Lambertian::scatter(const Ray& r_in, const HitRecord& rec, glm::vec4& attenuation, Ray& scattered) const
{
    glm::vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = Ray(rec.p, target - rec.p);
    attenuation = glm::vec4(albedo, 1.0f);
    return true;
}
