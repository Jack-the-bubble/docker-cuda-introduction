#include <iostream>
#include <fstream>
#include <filesystem>

#include "Vec3.h"

int main () {
    int nx = 200;
    int ny = 100;
    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../CPP/rt-in-one-weekend/artifacts/simple.ppm";
    simple_ppm.open(file_path);
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            RT::Vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());
            simple_ppm << ir << " " << ig << " " << ib << "\n";
        }
    }
    simple_ppm.close();
    return 0;
}