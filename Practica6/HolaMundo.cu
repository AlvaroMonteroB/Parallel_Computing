#include <stdio.h>
#include <cuda_runtime.h>

__global__ void HolaDesdeGPU()
{
    printf("Hola mundo desde GPU (device o dispositivo)!\n");
}

int main(void)
{
    printf("Hola mundo desde CPU (host)!\n");

    HolaDesdeGPU<<<2,10>>>();
    
    cudaDeviceSynchronize();
    
    return 0;
}

