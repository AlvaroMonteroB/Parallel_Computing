#include <stdio.h>
#include <cuda_runtime.h>
#include <time.h>
int Tam=10;
void most_mat(float mat[10][10]);
void multiply_matrices(float A[10][10], float B[10][10], float *C);


__global__ void mat_mul(float *mat1, float *mat2, float *mat3,int tam);

int main(){
    srand(time(NULL));
    float mat1[10][10];
    float mat2[10][10];
    float mat3[10][10]={{0,0,0},{0,0,0},{0,0,0}};
    float mat3_fake[10][10];
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            mat1[i][j]=rand()%10;
            mat2[i][j]=rand()%10;
        }
        
    }
    printf("Matriz 1\n");
    most_mat(mat1);
    printf("Matriz 2\n");
    most_mat(mat2);
    float *devA, *devB, *devC;
    dim3 threadsPerBlock(10, 10);
    dim3 numBlocks(1, 1);
    cudaMalloc((void**)&devA,Tam*Tam*sizeof(float));
    cudaMalloc((void**)&devB,Tam*Tam*sizeof(float));
    cudaMalloc((void**)&devC,Tam*Tam*sizeof(float));
    
    cudaMemcpy(devA,mat1,Tam*Tam*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(devB,mat2,Tam*Tam*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(devC,mat3,Tam*Tam*sizeof(int),cudaMemcpyHostToDevice);

    mat_mul<<<numBlocks,threadsPerBlock>>>(devA,devB,devC,Tam);

    cudaMemcpy(mat3,devC,Tam*Tam*sizeof(int),cudaMemcpyDeviceToHost);

    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);

   cudaDeviceReset();
    multiply_matrices(mat1,mat2,*mat3_fake);
    bool band=1;
    //printf("%f y %f\n",mat3[0][0],mat3_fake[0][0]);
    
    printf("Matriz cuda\n");
    most_mat(mat3);
    printf("Matriz cpu\n");
    most_mat(mat3_fake);


    for(int i = 0; i<10; i++){
        for (int j= 0; j<10; j++)
        {
            if(mat3[i][j]!=mat3_fake[i][j]){
                printf("Los resultados no son iguales\n");
                band=0;
                printf("%f y %f en (%i,%i) \n",mat3[i][j],mat3_fake[i][j],i,j);
                break;
            }
        }
        if(!band){
            break;
        }
    }

    return 0;
}





void most_mat(float mat[10][10]){
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%f ",mat[i][j]);
        }
            printf("\n");
        
    }
    return;
}


__global__ void mat_mul(float *mat1, float *mat2, float *mat3,int tam){
    int n=/*blockIdx.x * blockDim.x + */threadIdx.x;//NUmero de columna
    int m=/*blockIdx.y * blockDim.y + */threadIdx.y;//numero de fila de mat 1 o mat 3
    float sum=0.0f;
    for (int i = 0; i < tam; i++)
    {
        sum+=mat1[m*tam+i]*mat2[i*tam+n];
    }
    mat3[n*tam+m]=sum;
}


void multiply_matrices(float A[10][10], float B[10][10], float *C) {
    // Inicializamos la matriz C con ceros
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            C[i+j*Tam] = 0;
        }
    }
    
    // Multiplicación de matrices
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                C[i+j*Tam] += A[i][k] * B[k][j];
            }
        }
    }
}