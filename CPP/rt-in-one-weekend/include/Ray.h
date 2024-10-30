#ifndef RT_RAY_H
#define RT_RAY_H

#include "Vec3.h"

namespace RT {

class Ray {
public:
    Ray() = default;
    Ray(const Vec3 &a, const Vec3 &b): origin_(a), direction_(b) {}
    Vec3 origin() const { return origin_;}
    Vec3 direction() const { return direction_;}
    Vec3 point_at_parameter(float t) const {
        return origin_ + t * direction_;
    }

private:
    Vec3 origin_;
    Vec3 direction_;
};


} // namespace RT


#endif  // RT_RAY_H