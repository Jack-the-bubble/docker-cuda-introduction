#ifndef RT_LAMBERTIAN_H
#define RT_LAMBERTIAN_H

#include "Material.h"
#include "Utils.h"

namespace RT{

class Lambertian: public Material {
public:
    Lambertian(const Vec3 &al): albedo_(al) {}
    virtual bool scatter(const Ray &ray_in, const HitRecord &record, Vec3 &attenuation, Ray &scattered) const 
    {
        // random bounce to a sphere tangent to hit point
        Vec3 target = record.point + record.normal + get_random_in_unit_sphere();
        scattered = Ray(record.point, target - record.point);  // not sure what's going on here
        attenuation = albedo_;
        return true;
    }

private:
    Vec3 albedo_;
};

} // namespace RT


#endif