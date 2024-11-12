#include <iostream>
#include <fstream>
#include <filesystem>
#include "Utils.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Camera.h"

int main() {
   const int nx = 200;
    int ny = nx / 2;
    int ns = 100;  // antialiasing rays for a single pixel
    RT::Vec3 lower_left_corner = {-2.0, -1.0, -1.0};
    RT::Vec3 horizontal = {4.0, 0.0, 0.0};
    RT::Vec3 vertical = {0.0, 2.0, 0.0};
    RT::Vec3 origin = {0.0, 0.0, 0.0};
    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../CPP/rt-in-one-weekend/artifacts/chapter-10.ppm";
    simple_ppm.open(file_path);
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";

    const float R = cos(M_PI / 4);
    RT::Material *lambertian_1 = new RT::Lambertian({0.8, 0.3, 0.3});
    RT::Material *lambertian_2 = new RT::Lambertian({0.8, 0.8, 0.0});
    RT::Material *metal_1 = new RT::Metal({0.8, 0.6, 0.2}, 0.2);
    RT::Material *dielectric = new RT::Dielectric(1.5);
    RT::Hitable *list[4];
    list[0] = new RT::Sphere(RT::Vec3(0, 0, -1), 0.5, lambertian_1);
    list[1] = new RT::Sphere(RT::Vec3(0, -100.5, -1), 100, lambertian_2);
    list[2] = new RT::Sphere(RT::Vec3(1, 0, -1), 0.5, metal_1);
    list[3] = new RT::Sphere(RT::Vec3(-1, 0, -1), 0.5, dielectric);
    RT::Hitable *world = new RT::HitableList(list, 4);
    RT::Camera cam({-2, 2, 1}, {0, 0, -1}, {0, 1, 0}, 45, float(nx) / float(ny));
    // RT::Camera cam({0, 0, 1}, {0, 0, -1}, {0, 1, 0}, 90, float(nx) / float(ny));

    for (int j = ny - 1; j >=0; j--) {
        for (int i = 0; i < nx; i++) {
            RT::Vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                // antialiasing step: kinda wasteful when we're doing it for the whole image and not just the edges
                const float u = float(i + get_random()) / float(nx);
                const float v = float(j + get_random()) / float(ny);
                RT::Ray ray = cam.get_ray(u, v);
                RT::Vec3 point = ray.point_at_parameter(2.0);  // distance from camera?
                col += color(ray, world, 0);
            }
            col /= float(ns);
            col = RT::Vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
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
    delete list[2];
    delete list[3];
    delete lambertian_1;
    delete lambertian_2;
    delete metal_1;
    delete dielectric;
    return 0;
}
