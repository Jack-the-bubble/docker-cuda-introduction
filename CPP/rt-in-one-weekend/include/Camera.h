#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "Ray.h"

namespace RT {

struct CameraParams
{
    Vec3 lookfrom;
    Vec3 lookat;
    Vec3 vector_up;
    float vertical_fov;
    float aspect_ratio;
    float aperture;
    float focus_dist;
};

class Camera {
public:
    Camera(const CameraParams &params): lens_radius_(params.aperture / 2), origin_(params.lookfrom)
    {
        // Vec3 u, v, w;
        const float theta = params.vertical_fov * M_PI / 180;
        const float half_height = tan(theta / 2);  // from proportions
        const float half_width = params.aspect_ratio * half_height;
        w = unit_vector(params.lookfrom - params.lookat);
        u = unit_vector(cross(params.vector_up, w));
        v = cross(w, u);
        lower_left_corner_ = origin_ - half_width * params.focus_dist * u - half_height * params.focus_dist * v - params.focus_dist * w;
        horizontal_ = 2 * half_width * params.focus_dist * u;
        vertical_ = 2 * half_height * params.focus_dist * v;
    }

    Ray get_ray(const float s, const float t)
    {
        Vec3 random_origin = lens_radius_ * get_random_in_unit_disk();
        Vec3 offset = u * random_origin.x() + v * random_origin.y();

        return Ray(origin_ + offset, lower_left_corner_ + s*horizontal_ + t*vertical_ - origin_ - offset);
    }

private:
    Vec3 origin_;
    Vec3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 u, v, w;
    const float lens_radius_;
};
} // namespace RT

#endif // RT_CAMERA_H
