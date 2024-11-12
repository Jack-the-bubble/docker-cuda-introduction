#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "Ray.h"

namespace RT {

class Camera {
public:
    Camera(const Vec3 &lookfrom, const Vec3 &lookat, const Vec3 &vector_up, const float vertical_fov, const float aspect_ratio) //: origin_(lookfrom)
    {
        Vec3 u, v, w;
        const float theta = vertical_fov * M_PI / 180;
        const float half_height = tan(theta / 2);  // from proportions
        const float half_width = aspect_ratio * half_height;
        origin_ = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vector_up, w));
        v = cross(w, u);
        lower_left_corner_ = origin_ - half_width * u - half_height * v - w;
        horizontal_ = 2 * half_width * u;
        vertical_ = 2 * half_height * v;
    }

    Camera(const float vertical_fov, const float aspect_ratio, const Vec3 &origin): origin_(origin)
    {
        const float theta = vertical_fov * M_PI / 180;
        const float half_height = tan(theta / 2);  // from proportions
        const float half_width = aspect_ratio * half_height;
        lower_left_corner_ = Vec3(-half_width, -half_height, -1.0);  // -1 in z coords
        horizontal_ = Vec3(2 * half_width, 0, 0);
        vertical_ = Vec3(0, 2 * half_height, 0);

    }
    Camera(const Vec3 &low_left_corner, const Vec3 &hor, const Vec3 &ver, const Vec3 &origin):
            lower_left_corner_(low_left_corner), horizontal_(hor), vertical_(ver), origin_(origin)
    {}
    Camera() = default;
    ~Camera() = default;

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
