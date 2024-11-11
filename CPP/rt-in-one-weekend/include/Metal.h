#ifndef RT_METEAL_H
#define RT_METAL_H

#include "Material.h"
#include "Utils.h"

namespace RT{

class Metal: public Material {
public:
    Metal(const Vec3 &al, const float fuzziness): albedo_(al)
    {
        if (fuzziness < 1)
        {
            fuzz_ = fuzziness;
        }
        else
        {
            fuzz_ = 1;
        }
    }

    Vec3 reflect(const Vec3 &v, const Vec3 &n) const
    {
        return v - 2 * dot(v, n) * n;
    }

    virtual bool scatter(const Ray &ray_in, const HitRecord &record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 reflected = reflect(unit_vector(ray_in.direction()), record.normal);
        scattered = Ray(record.point, reflected + fuzz_ * get_random_in_unit_sphere());  // not sure what's going on here
        attenuation = albedo_;
        return (dot(scattered.direction(), record.normal) > 0);  // not sure what's going on here
    }

private:
    Vec3 albedo_;
    float fuzz_;
};

} // namespace RT


#endif