#include <iostream>
#include <fstream>
#include <filesystem>

#include "Utils.h"
#include "Ray.h"

__device__ RT::Vec3 color(const RT::Ray &r) {
    RT::Vec3 unit_direction = RT::unit_vector(r.direction());
    // scaling from (-1;1) to (0:2)
    const float t = 0.5f * (unit_direction.y() + 1.0f);
    const RT::Vec3 start_value = {1.0, 1.0, 1.0};
    const RT::Vec3 end_value = {0.5, 0.0, 1.0};
    // linear blend interpolation (lerp)
    RT::Vec3 ret_vector = (1.0f - t) * start_value + t * end_value;
    return ret_vector;
}

__global__
void render(RT::Vec3 *float_buffer, int nx, int ny,
            RT::Vec3 lower_left_corner, RT::Vec3 horizontal,
            RT::Vec3 vertical, RT::Vec3 origin)
{
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    if (index_x >= nx || index_y >= ny)
    {
        return;
    }
    const int pixel_idx = index_y * nx + index_x;
    const float u = float(index_x) / float(nx);
    const float v = float(index_y) / float(ny);
    RT::Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
    auto pixel_color = color(ray);
    float_buffer[pixel_idx] = pixel_color;
}

int main() {
    const int nx = 200;
    int ny = 100;
    RT::Vec3 lower_left_corner = {-2.0, -1.0, -1.0};
    RT::Vec3 horizontal = {4.0, 0.0, 0.0};
    RT::Vec3 vertical = {0.0, 2.0, 0.0};
    RT::Vec3 origin = {0.0, 0.0, 0.0};
    const int num_pixels = nx * ny;
    size_t fb_size = num_pixels *sizeof(RT::Vec3);

    // allocate fb_size on gpu
    RT::Vec3 *fb;
    checkCudaErrors(cudaMallocManaged((void **)&fb, fb_size));

    int tx = 8;  // x block size
    int ty = 8;  // y block size

    dim3 blocks(nx/tx + 1, ny/ty + 1);
    dim3 threads(tx, ty);

    render<<<blocks, threads>>>(fb, nx, ny, lower_left_corner, horizontal,
                                vertical, origin);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../GPU/artifacts/chapter-3.ppm";
    simple_ppm.open(file_path);
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >=0; j--) {
        for (int i = 0; i < nx; i++) {
            const size_t pixel_index = j * nx + i ;
            const float r = fb[pixel_index].r();
            const float g = fb[pixel_index].g();
            const float b = fb[pixel_index].b();
            const int ir = int(255.99 * r);
            const int ig = int(255.99 * g);
            const int ib = int(255.99 * b);
            simple_ppm << ir << " " << ig << " " << ib << "\n";
        }
    }
    simple_ppm.close();

    checkCudaErrors(cudaFree(fb));
    return 0;

}