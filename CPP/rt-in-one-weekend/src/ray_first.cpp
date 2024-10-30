#include <iostream>
#include <fstream>
#include <filesystem>

#include "Ray.h"

RT::Vec3 color(const RT::Ray &r) {
    RT::Vec3 unit_direction = RT::unit_vector(r.direction());
    // scaling from (-1;1) to (0:2)
    const float t = 0.5 * (unit_direction.y() + 1.0);
    const RT::Vec3 start_value = {1.0, 1.0, 1.0};
    const RT::Vec3 end_value = {0.5, 0.0, 1.0};
    // linear blend interpolation (lerp)
    RT::Vec3 ret_vector = (1.0 - t) * start_value + t * end_value;
    return ret_vector;
}

int main() {
    const int nx = 200;
    int ny = 100;
    RT::Vec3 lower_left_corner = {-2.0, -1.0, -1.0};
    RT::Vec3 horizontal = {4.0, 0.0, 0.0};
    RT::Vec3 vertical = {0.0, 2.0, 0.0};
    RT::Vec3 origin = {0.0, 0.0, 0.0};
    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../CPP/rt-in-one-weekend/artifacts/blue-ray.ppm";
    simple_ppm.open(file_path);
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >=0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            RT::Ray r = {origin, lower_left_corner + u * horizontal + v * vertical};
            RT::Vec3 col = color(r);
            const int ir = int(255.99 * col.r());
            const int ig = int(255.99 * col.g());
            const int ib = int(255.99 * col.b());
            simple_ppm << ir << " " << ig << " " << ib << "\n";
        }
    }
    simple_ppm.close();
    return 0;

}