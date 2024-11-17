#include <iostream>
#include <fstream>
#include <filesystem>
#include <float.h>

#include <curand_kernel.h>

#include "Utils.h"
#include "Sphere.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Camera.h"
#include "Ray.h"

__device__ RT::Vec3 color(const RT::Ray &ray, RT::Hitable **d_world) {
    // return normal vector converted to color if hit sphere
    RT::HitRecord record;
    if ((*d_world)->hit(ray, 0.0, MAXFLOAT, record)) {
        float r1 = record.normal.x();
        float r2 = record.normal.y();
        float r3 = record.normal.z();
        // mapping normal to color and scaling to (0, 1)
        RT::Vec3 ret_color = 0.5f * RT::Vec3(record.normal.x() + 1.0f,
                                             record.normal.y() + 1.0f,
                                             record.normal.z() + 1.0f);
        return ret_color;
    }
    RT::Vec3 unit_direction = RT::unit_vector(ray.direction());
    // scaling from (-1;1) to (0:2)
    const float t = 0.5f * (unit_direction.y() + 1.0f);
    const RT::Vec3 start_value = {1.0, 1.0, 1.0};
    const RT::Vec3 end_value = {0.5, 0.0, 1.0};
    // linear blend interpolation (lerp)
    RT::Vec3 ret_vector = (1.0f - t) * start_value + t * end_value;
    return ret_vector;
}

__global__
void render_init(const int max_x, const int max_y, curandState *d_rand_state)
{
    const int i = threadIdx.x + blockIdx.x * blockDim.x;
    const int j = threadIdx.y + blockIdx.y * blockDim.y;
    if (i >= max_x or j >= max_y)
    {
        return;
    }
    const int pixel_idx = j * max_x + i;
    // each thread gets the same seed, different sequence number and no offset
    curand_init(1984, pixel_idx, 0, &d_rand_state[pixel_idx]);
}

__global__
void render(RT::Vec3 *float_buffer, int nx, int ny, int ns, RT::Camera **cam,
            RT::Hitable **d_world, curandState *rand_state)
{
    *cam = new RT::Camera();
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    if (index_x >= nx || index_y >= ny)
    {
        return;
    }
    int pixel_idx = index_y * nx + index_x;
    curandState local_rand_state = rand_state[pixel_idx];
    RT::Vec3 col(0, 0, 0);
    RT::Ray ray = (*cam)->get_ray(0, 1);
    for (int s = 0; s < ns; s++) {
        float f1 = curand_uniform(&local_rand_state);
        float f2 = curand_uniform(&local_rand_state);
        // antialiasing step: kinda wasteful when we're doing it for the whole image and not just the edges
        const float u = float(index_x + curand_uniform(&local_rand_state)) / float(nx);
        const float v = float(index_y + curand_uniform(&local_rand_state)) / float(ny);
        RT::Ray ray = (*cam)->get_ray(u, v);
        col += color(ray, d_world);
    }
    col /= float(ns);
    float col1 = col.r();
    float col2 = col.g();
    float col3 = col.b();
    // RT::Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);

    // auto pixel_color = color(ray, d_world);
    float_buffer[pixel_idx] = col;
}

__global__
void create_world(RT::Hitable **d_list, RT::Hitable **d_world, RT::Camera **d_camera)
{
    if (threadIdx.x == 0 && blockIdx.x == 0)
    {
        *(d_list) = new RT::Sphere({0, 0, -1}, 0.5);
        *(d_list + 1) = new RT::Sphere({0, -100.5, -1}, 100);
        *d_world = new RT::HitableList(d_list, 2);
        // *d_camera = new RT::Camera();
        *d_camera = new RT::Camera({-2.0, -1.0, -1.0}, {4.0, 0.0, 0.0}, {0.0, 2.0, 0.0}, {0.0, 0.0, 0.0});
    }
}

__global__
void free_world(RT::Hitable **d_list, RT::Hitable **d_world, RT::Camera **d_camera)
{
    delete *(d_list);
    delete *(d_list + 1);
    delete *d_world;
    delete *d_camera;
}

int main() {
    const int nx = 200;
    int ny = nx / 2;
    int ns = 100;
    // RT::Vec3 lower_left_corner = {-2.0, -1.0, -1.0};
    // RT::Vec3 horizontal = {4.0, 0.0, 0.0};
    // RT::Vec3 vertical = {0.0, 2.0, 0.0};
    // RT::Vec3 origin = {0.0, 0.0, 0.0};
    const int num_pixels = nx * ny;
    size_t fb_size = num_pixels *sizeof(RT::Vec3);

    // setup random state for each pixel
    curandState *d_rand_state;
    checkCudaErrors(cudaMalloc((void **)&d_rand_state, num_pixels * sizeof(curandState)));

    // create world on the gpu
    RT::Hitable **d_list;  // device list
    checkCudaErrors(cudaMalloc((void **)&d_list, 2 * sizeof(RT::Hitable *)));
    RT::Hitable **d_world; // world stored on device
    checkCudaErrors(cudaMalloc((void **)&d_world, sizeof(RT::Hitable *)));
    RT::Camera **d_camera;
    checkCudaErrors(cudaMalloc((void **)&d_camera, sizeof(RT::Camera *)));
    create_world<<<1, 1>>>(d_list, d_world, d_camera);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    // allocate fb_size on gpu
    RT::Vec3 *fb;
    checkCudaErrors(cudaMallocManaged((void **)&fb, fb_size));


    int tx = 8;  // x block size
    int ty = 8;  // y block size

    dim3 blocks(nx/tx + 1, ny/ty + 1);
    dim3 threads(tx, ty);

    // initialize random values
    render_init<<<blocks, threads>>>(nx, ny, d_rand_state);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    render<<<blocks, threads>>>(fb, nx, ny, ns, d_camera, d_world, d_rand_state);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    std::ofstream simple_ppm;
    const auto file_path = std::filesystem::current_path() / "../GPU/artifacts/chapter-6.ppm";
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

    free_world<<<1, 1>>>(d_list, d_world, d_camera);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaFree(fb));
    checkCudaErrors(cudaFree(d_list));
    checkCudaErrors(cudaFree(d_world));
    checkCudaErrors(cudaFree(d_camera));
    checkCudaErrors(cudaFree(d_rand_state));

    cudaDeviceReset();

    return 0;

}