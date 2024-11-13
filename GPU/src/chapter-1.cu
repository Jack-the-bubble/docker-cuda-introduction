#include <fstream>

#include "Utils.h"


__global__
void render(float *float_buffer, const int nx, const int ny)
{
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    if (index_x >= nx || index_y >= ny)
    {
        return;
    }
    const int pixel_idx = index_y * nx * 3 + index_x * 3;
    float_buffer[pixel_idx] = float(index_x) / float(nx);
    float_buffer[pixel_idx + 1] = float(index_y) / float(ny);
    float_buffer[pixel_idx + 2] = 0.2;
}

int main(void)
{
    const int nx = 200;
    const int ny = 100;
    const int num_pixels = nx * ny;
    size_t fb_size = 3 * num_pixels *sizeof(float);

    // allocate fb_size on gpu
    float *fb;
    checkCudaErrors(cudaMallocManaged((void **)&fb, fb_size));

    int tx = 8;  // x block size
    int ty = 8;  // y block size

    dim3 blocks(nx/tx + 1, ny/ty + 1);
    dim3 threads(tx, ty);
    render<<<blocks, threads>>>(fb, nx, ny);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    std::ofstream simple_ppm;
    simple_ppm.open("/home/cuda_ws/GPU/artifacts/chapter-1.ppm");
    simple_ppm << "P3\n" <<nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            const size_t pixel_index = j * 3 *nx + i * 3;
            const float r = fb[pixel_index + 0];
            const float g = fb[pixel_index + 1];
            const float b = fb[pixel_index + 2];
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