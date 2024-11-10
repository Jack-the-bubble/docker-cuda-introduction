#include <iostream>
#include <fstream>
#include <filesystem>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include <random>
#include <stdlib.h>
// #include "float.h"  //used for MAXFLOAT, why not numeric_limits?
#include <limits>  // in case of problems later, use float.h for MAXFLOAT

RT::Vec3 color(const RT::Ray &ray, const RT::Hitable *world)
{
    RT::HitRecord record;
    const float max_float = std::numeric_limits<float>::max();
    if (world->hit(ray, 0.0, max_float, record))
    {
        // mapping normal to color and scaling to (0, 1)
        RT::Vec3 ret_color = 0.5 * RT::Vec3(record.normal.x() + 1, record.normal.y() + 1, record.normal.z() + 1);
        return ret_color;
    }
    
    RT::Vec3 unit_direction = RT::unit_vector(ray.direction());
    // scaling from (-1;1) to (0:1) again
    const float t = 0.5 * (unit_direction.y() + 1.0);
    const RT::Vec3 start_value = {1.0, 1.0, 1.0};
    const RT::Vec3 end_value = {0.5, 0.7, 1.0};
    // linear blend interpolation (lerp)
    RT::Vec3 ret_vector = (1.0 - t) * start_value + t * end_value;
    return ret_vector;
}

float get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 1); // range [0, 1)
    return dis(e);
}

int main() {
   const int nx = 200;
    int ny = nx / 2;
    int ns = 100;
    RT::Vec3 lower_left_corner = {-2.0, -1.0, -1.0};
    RT::Vec3 horizontal = {4.0, 0.0, 0.0};
    RT::Vec3 vertical = {0.0, 2.0, 0.0};
    RT::Vec3 origin = {0.0, 0.0, 0.0};
    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../CPP/rt-in-one-weekend/artifacts/chapter-6.ppm";
    simple_ppm.open(file_path);
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";

    RT::Hitable *list[2];
    list[0] = new RT::Sphere(RT::Vec3(0, 0, -1), 0.5);
    list[1] = new RT::Sphere(RT::Vec3(0, -100.5, -1), 100);
    RT::Hitable *world = new RT::HitableList(list, 2);
    RT::Camera cam({-2.0, -1.0, -1.0}, {4.0, 0.0, 0.0}, {0.0, 2.0, 0.0}, {0.0, 0.0, 0.0});
    for (int j = ny - 1; j >=0; j--) {
        for (int i = 0; i < nx; i++) {
            RT::Vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                // antialiasing step: kinda wasteful when we're doing it for the whole image and not just the edges
                const float u = float(i + get_random()) / float(nx);
                const float v = float(j + get_random()) / float(ny);
                RT::Ray ray = cam.get_ray(u, v);
                RT::Vec3 point = ray.point_at_parameter(2.0);  // distance from camera?
                col += color(ray, world);
            }
            col /= float(ns);
            const int ir = int(255.99 * col.r());
            const int ig = int(255.99 * col.g());
            const int ib = int(255.99 * col.b());
            simple_ppm << ir << " " << ig << " " << ib << "\n";
        }
    }
    simple_ppm.close();

    delete world;
    delete list[0];
    delete list[1];
    return 0;
}
