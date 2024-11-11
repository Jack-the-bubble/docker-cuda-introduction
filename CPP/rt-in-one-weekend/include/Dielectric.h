/**
 * Material class that always refracts light when possible - no reflections in a glass ball!
 */

#include "Material.h"

#ifndef RT_DIELECTRIC_H
#define RT_DIELECTRIC_H

namespace RT {
class Dielectric: public Material
{
public:
    Dielectric(const float ri): refraction_index_(ri) {}

    float getSchlick(const float cosine, const float refraction_idx) const
    {
        float r0 = (1 - refraction_idx) / (1 + refraction_idx);
        r0 *= r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

    virtual bool scatter(const Ray &ray_in, const HitRecord &record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 outward_normal;
        Vec3 reflected = reflect(ray_in.direction(), record.normal);
        float ni_over_nt;
        attenuation = Vec3(1.0, 1.0, 1.0);  // glass surface absorbs nothing, so set values to 1, also zero at blue channel, apparently it helps to find some sort of a bug
        Vec3 refracted;
        float reflect_probability;
        float cosine;
        if (dot(ray_in.direction(), record.normal) > 0)
        {
            outward_normal = -record.normal;
            ni_over_nt = refraction_index_;
            cosine = refraction_index_ * dot(ray_in.direction(), record.normal) / ray_in.direction().length();
        }
        else
        {
            outward_normal = record.normal;
            ni_over_nt = 1.0 / refraction_index_;
            cosine = -dot(ray_in.direction(), record.normal) / ray_in.direction().length();
        }

        if (refract(ray_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            reflect_probability = getSchlick(cosine, refraction_index_);
        }
        else
        {
            reflect_probability = 1.0;
        }

        if (get_random() < reflect_probability)
        {
            scattered = Ray(record.point, reflected);
        }
        else
        {
            scattered = Ray(record.point, refracted);
        }

        return true;
    }
private:
    const float refraction_index_;
};
} // namespace RT

#endif