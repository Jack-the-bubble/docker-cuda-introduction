#ifndef RT_HITABLE_H
#define RT_HITABLE_H

#include "Ray.h"

namespace RT
{
struct HitRecord
{
    float t; // scaling factor from A+tB
    Vec3 point; // originally p - ray? direction? point of hit?
    Vec3 normal;
};

class Hitable
{
public:
    virtual bool hit(const Ray &ray, const float t_min, const float t_max, HitRecord &record) const = 0;
};

} // namespace RT

#endif // RT_HITABLE_H