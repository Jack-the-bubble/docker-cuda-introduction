#include <iostream>
#include <fstream>
#include <filesystem>
#include "Sphere.h"
#include "HitableList.h"
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


int main() {
   const int nx = 200;
    int ny = nx / 2;
    RT::Vec3 lower_left_corner = {-2.0, -1.0, -1.0};
    RT::Vec3 horizontal = {4.0, 0.0, 0.0};
    RT::Vec3 vertical = {0.0, 2.0, 0.0};
    RT::Vec3 origin = {0.0, 0.0, 0.0};
    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../CPP/rt-in-one-weekend/artifacts/chapter-5.ppm";
    simple_ppm.open(file_path);
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";

    RT::Hitable *list[2];
    list[0] = new RT::Sphere(RT::Vec3(0, 0, -1), 0.5);
    list[1] = new RT::Sphere(RT::Vec3(0, -100.5, -1), 100);
    RT::Hitable *world = new RT::HitableList(list, 2);
    for (int j = ny - 1; j >=0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            RT::Ray ray = {origin, lower_left_corner + u * horizontal + v * vertical};

            RT::Vec3 p = ray.point_at_parameter(2.0);  // I wonder why this distance?
            RT::Vec3 col = color(ray, world);
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
