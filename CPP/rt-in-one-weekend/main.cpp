#include <iostream>
#include <fstream>

int main () {
    int nx = 200;
    int ny = 100;
    std::ofstream simple_ppm;
    simple_ppm.open("/home/cuda_ws/CPP/rt-in-one-weekend/artifacts/simple.ppm");
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            simple_ppm << ir << " " << ig << " " << ib << "\n";

        }
    }

    simple_ppm.close();
    return 0;
}