#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "Hitable.h"

namespace RT
{

class Sphere: public Hitable
{
public:
    Sphere() = default;
    ~Sphere() = default;
    Sphere(const Vec3 &center, const float radius): center_(center), radius_(radius) {}

    virtual bool hit(const Ray &ray, const float t_min, const float t_max, HitRecord &record) const;

private:
    Vec3 center_;
    float radius_;
};
} // namespace RT

#endif // RT_SPHERE_H