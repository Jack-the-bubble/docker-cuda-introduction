#include "Sphere.h"

namespace RT
{

bool Sphere::hit(const Ray &ray, const float t_min, const float t_max, HitRecord &record) const
{
    const Vec3 circle_origin = ray.origin() - center_;  // what is this?
    // parameters of quadratic equation at^2 + bt + c (notes chapter 4)
    const float a = RT::dot(ray.direction(), ray.direction());
    const float b = RT::dot(circle_origin, ray.direction());
    const float c = RT::dot(circle_origin, circle_origin) - radius_ * radius_;
    const float delta = b * b - a * c;

    if (delta > 0)
    {
        float temp = (-b - sqrt(b*b - a * c)) / a;  // closer point hitting the sphere
        if (temp < t_max && temp > t_min)
        {
            record.t = temp;
            record.point = ray.point_at_parameter(record.t);
            record.normal = (record.point - center_) / radius_;
            return true;
        }
        temp = (-b + sqrt(b*b - a * c)) / a;  // farther point hitting the sphere
        if (temp < t_max && temp > t_min)
        {
            record.t = temp;
            record.point = ray.point_at_parameter(record.t);
            record.normal = (record.point - center_) / radius_;
            return true;
        }
    }
    return false;
}
} // namespace RT
