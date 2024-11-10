#ifndef RT_UTILS_H
#define RT_UTILS_H

#include <random>
#include <limits>

#include "Hitable.h"

float get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 1); // range [0, 1)
    return dis(e);
}

namespace RT {

Vec3 get_random_in_unit_sphere()
{
    Vec3 point;
    do
    {
        point = 2.0 * Vec3(get_random(), get_random(), get_random()) - Vec3(1, 1, 1);
    } while (point.squared_length() >= 1.0);
    return point;
}

Vec3 color(const Ray &ray, const Hitable *world)
{
    HitRecord record;
    const float max_float = std::numeric_limits<float>::max();
    if (world->hit(ray, 0.001, max_float, record))
    {
        Vec3 target = record.point + record.normal + get_random_in_unit_sphere();
        // mapping normal to color and scaling to (0, 1)
        Vec3 ret_color = 0.5 * color(Ray(record.point, target-record.point), world);
        return ret_color;
    }

    Vec3 unit_direction = unit_vector(ray.direction());
    // scaling from (-1;1) to (0:1) again
    const float t = 0.5 * (unit_direction.y() + 1.0);
    const Vec3 start_value = {1.0, 1.0, 1.0};
    const Vec3 end_value = {0.5, 0.7, 1.0};
    // linear blend interpolation (lerp)
    Vec3 ret_vector = (1.0 - t) * start_value + t * end_value;
    return ret_vector;
}



} // namespace RT


#endif