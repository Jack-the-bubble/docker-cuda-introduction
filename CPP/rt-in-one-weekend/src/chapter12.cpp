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

RT::Hitable *random_scene()
{
    const int n = 500;
    RT::Hitable **list = new RT::Hitable*[n+1];
    list[0] = new RT::Sphere({0, -1000, 0}, 1000, new RT::Lambertian({0.5, 0.5, 0.5}));  //defining the wolrd sphere
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            const float choose_material = get_random();
            const RT::Vec3 center(a + 0.9 * get_random(), 0.2, b + 0.9 * get_random());
            if ((center - RT::Vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_material < 0.8)  // diffuse material
                {
                    list[i++] = new RT::Sphere(center, 0.2, new RT::Lambertian({get_random() * get_random(), get_random() * get_random(), get_random() * get_random()}));
                }
                else if (choose_material < 0.95) // metal
                {
                    float x = 0.5 * (1 + get_random());
                    float y = 0.5 * (1 + get_random());
                    float z = 0.5 * (1 + get_random());
                    list[i++] = new RT::Sphere(center, 0.2, new RT::Metal({x, y, z}, 0.5 * get_random()));
                }
                else  // glass
                {
                    list[i++] = new RT::Sphere(center, 0.2, new RT::Dielectric(1.5));
                }
            }
        }
    }

    // add 3 big-ass spheres
    list[i++] = new RT::Sphere({0, 1, 0}, 1.0, new RT::Dielectric(1.5));
    list[i++] = new RT::Sphere({-4, 1, 0}, 1.0, new RT::Lambertian({0.4, 0.2, 0.1}));
    list[i++] = new RT::Sphere({4, 1, 0}, 1.0, new RT::Metal({0.7, 0.6, 0.5}, 0.0));

    return new RT::HitableList(list, i);
}

int main() 
{
   const int nx = 1000;
    int ny = nx / 2;
    int ns = 50;  // antialiasing rays for a single pixel
    RT::Vec3 lower_left_corner = {-2.0, -1.0, -1.0};
    RT::Vec3 horizontal = {4.0, 0.0, 0.0};
    RT::Vec3 vertical = {0.0, 2.0, 0.0};
    RT::Vec3 origin = {0.0, 0.0, 0.0};
    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../CPP/rt-in-one-weekend/artifacts/chapter-12.ppm";
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
    RT::Hitable *world = random_scene();

    // big aperture
    RT::Vec3 lookfrom = {3, 1, 3};
    RT::Vec3 lookat = {0, 0, -1};
    const float dist_to_focus = (lookfrom - lookat).length();
    const float aperture = 0.05;
    RT::CameraParams cam_params =
    {
        lookfrom,
        lookat,
        {0, 1, 0},  // vector up
        90, // vertical fov
        float(nx) / float(ny), // aspect ratio
        aperture,
        dist_to_focus
    };

    RT::Camera cam(cam_params);

    for (int j = ny - 1; j >=0; j--) 
    {
        std::cout<<ny - j <<"/"<<ny<<"\n";
        for (int i = 0; i < nx; i++) 
        {
            RT::Vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) 
            {
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
