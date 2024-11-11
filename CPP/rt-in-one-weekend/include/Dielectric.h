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

    virtual bool scatter(const Ray &ray_in, const HitRecord &record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 outward_normal;
        Vec3 reflected = reflect(ray_in.direction(), record.normal);
        float ni_over_nt;
        attenuation = Vec3(1.0, 1.0, 1.0);  // glass surface absorbs nothing, so set values to 1, also zero at blue channel, apparently it helps to find some sort of a bug
        Vec3 refracted;
        if (dot(ray_in.direction(), record.normal) > 0)
        {
            outward_normal = -record.normal;
            ni_over_nt = refraction_index_;
        }
        else
        {
            outward_normal = record.normal;
            ni_over_nt = 1.0 / refraction_index_;
        }

        if (refract(ray_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            scattered = Ray(record.point, refracted);
        }
        else
        {
            scattered = Ray(record.point, reflected);
            return false;
        }

        return true;
    }
private:
    const float refraction_index_;
};
} // namespace RT

#endif