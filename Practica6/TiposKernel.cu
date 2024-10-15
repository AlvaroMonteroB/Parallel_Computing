#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void suma_GPU(int a, int b, int *c)
{
   *c = a + b;
}

__host__ int suma_CPU(int a, int b)
{
   return (a + b);
}

int main(int argc, char** argv)
{

   int n1=1, n2=2, c=0;
   int *host_c;
   int m1=10, m2=20;
   int *dev_c;

   host_c=(int*)malloc(sizeof(int));

   cudaMalloc((void**)&dev_c,sizeof(int));

   c=suma_CPU(n1, n2);
   
   printf("\nCPU: %2d + %2d = %2d \n",n1, n2, c);

   suma_GPU<<<1,1>>>(m1, m2, dev_c);

   cudaMemcpy(host_c,dev_c,sizeof(int),cudaMemcpyDeviceToHost );

   printf("GPU: %2d + %2d = %2d \n",m1, m2, *host_c); 

   return 0;
}
