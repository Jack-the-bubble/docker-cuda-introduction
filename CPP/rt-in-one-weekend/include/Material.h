#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "Hitable.h"

namespace RT {
    
class Material {
public:
    virtual bool scatter(const Ray &ray_in, const HitRecord &record, Vec3 &attenuation, Ray &scattered) const = 0;
};
} // namespace RT


#endif