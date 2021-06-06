#include "dielectric.h"

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec, glm::vec4& attenuation, ray& scattered) const
{
    glm::vec3 outward_normal;
    glm::vec3 reflected = glm::reflect(r_in.direction, rec.normal);
    float ni_over_nt;
    attenuation = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float cosine;
    if (glm::dot(r_in.direction, rec.normal) > 0)
    {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        cosine = ref_idx * glm::dot(r_in.direction, rec.normal) / glm::length(r_in.direction);
    }
    else
    {
        outward_normal = rec.normal;
        ni_over_nt = 1.0f / ref_idx;
        cosine = -glm::dot(r_in.direction, rec.normal) / glm::length(r_in.direction);
    }
    glm::vec3 refracted = glm::refract(r_in.direction, outward_normal, ni_over_nt);
    float reflect_prob;
    if (glm::length(refracted) > 0.0f)
    {
        reflect_prob = schlick(cosine, ref_idx);
    }
    else
    {
        reflect_prob = 1.0f;
    }
    if (random_num() < reflect_prob)
    {
        scattered = ray(rec.p, reflected);
    }
    else
    {
        scattered = ray(rec.p, refracted);
    }
    return true;
}
