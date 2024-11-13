#ifndef GPU_CUUTILS_H
#define GPU_CUUTILS_H

#include <iostream>

#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__)
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line)
{
    if (result) 
    {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
        file << ":" <<line << " '" << func <<"' \n";
        // make sure we call cuda device reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}

#endif