#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "Hitable.h"

namespace RT {
    
class Material {
public:
    Vec3 reflect(const Vec3 &v, const Vec3 &n) const
    {
        return v - 2 * dot(v, n) * n;
    }

    bool refract(const Vec3 &v, const Vec3 &n, const float ni_over_nt, Vec3 &refracted) const
    {
        const Vec3 uv = unit_vector(v);
        const float dt = dot(uv, n);
        const float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
        if (discriminant > 0)
        {
            refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        }
        return false;
    }

    virtual bool scatter(const Ray &ray_in, const HitRecord &record, Vec3 &attenuation, Ray &scattered) const = 0;
};
} // namespace RT


#endif