#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "Ray.h"

namespace RT {

class Camera {
public:
    Camera() = default;
    ~Camera() = default;
    __device__
    Camera(const Vec3 &low_left_corner, const Vec3 &hor, const Vec3 &ver, const Vec3 &origin):
            lower_left_corner_(low_left_corner), horizontal_(hor), vertical_(ver), origin_(origin)
    {}
    // __device__
    // Camera()
    // {
    //     origin_ = {-2.0, -1.0, -1.0};
    //     lower_left_corner_ = {4.0, 0.0, 0.0};
    //     horizontal_ = {0.0, 2.0, 0.0};
    //     vertical_ = {0.0, 0.0, 0.0};
    // }

    __device__
    Ray get_ray(const float u, const float v)
    {
        return Ray(origin_, lower_left_corner_ + u*horizontal_ + v*vertical_ - origin_);
    }
private:
    Vec3 origin_;
    Vec3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
};
} // namespace RT

#endif // RT_CAMERA_H
