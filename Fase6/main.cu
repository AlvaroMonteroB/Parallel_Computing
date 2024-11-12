#include <stdio.h>
#include <cuda_runtime.h>
#include <time.h>
#include <math.h>
#define PI 3.14159265359
#define a0 4-((10*pow(PI,2))/9)


double *linspace(double ini, double fin, double step,int *size);
void write_buffer(double *space, double *vals,char * file, int space_size);
__global__ void calc_term(double *xrange, double*result, int tam);
__global__ void def_funct(double *matrix, double* reconstructed_funct, int tam);
__global__ void print_grid(double *matrix, int i, int j);



int main(){
    int n_max=24;
    int tam;
    double *x_range=linspace(-3.14,3.14,.1,&tam);
    double *lins_c, *mat_rec,*reconstructed_gpu;//Apuntadores en gpu
    double *result=(double*)calloc(tam*n_max,sizeof(double));//Matriz de resultados
    double *reconstructed_funct=(double*)malloc(tam*sizeof(double));//Función reconstruida
    for (int i = 0; i < tam; i++)
    {   
        reconstructed_funct[i]=a0/2-2;
    }
    cudaMalloc((void**)&lins_c,tam*sizeof(double));//Linspace
    cudaMalloc((void**)&reconstructed_gpu,tam*sizeof(double));//Resultante

    cudaMalloc((void**)&mat_rec,n_max * tam*sizeof(double));//Reconstruccion

    cudaMemcpy(lins_c,x_range,tam*sizeof(double),cudaMemcpyHostToDevice); //x_range ->lins_c
    cudaMemcpy(mat_rec,result,tam*sizeof(double),cudaMemcpyHostToDevice);//result ->mat_rec
    cudaMemcpy(reconstructed_gpu,reconstructed_funct,tam*sizeof(double),cudaMemcpyHostToDevice);// reconstructed -> result funct

    dim3 threadsPerBlock( n_max);
    dim3 numBlocks(tam);

    calc_term<<<numBlocks,threadsPerBlock>>>(lins_c,mat_rec,n_max);//Reconstruccion de la funcion
    dim3 threadsPerBlock2(tam);//longitud del linspace
    dim3 block1(1);
    def_funct<<<block1,threadsPerBlock2>>>(mat_rec,reconstructed_gpu,n_max);//FUncion resultante
    //print_grid<<<1,1>>>(mat_rec,tam,n_max);

    cudaMemcpy(reconstructed_funct,reconstructed_gpu,tam*sizeof(double),cudaMemcpyDeviceToHost);//reconstructed_gpu -> reconstructed
    for (int i = 0; i < tam; i++) {
            if(i==tam-1){
                printf("(%lf,%f)\n", reconstructed_funct[i],x_range[i]);
            }else{
                printf( "(%lf,%f), ", reconstructed_funct[i],x_range[i]);
            }

            
        }
    write_buffer(x_range,reconstructed_funct,"result.csv",tam);

    return 0;
}



__global__ void calc_term(double *xrange, double*result,int tam){
    int i = blockIdx.x ;//EL bloque nos dice la coordenada del linspace x
    //int m= blockIdx.x * blockDim.y + threadIdx.y;
    int n = threadIdx.x; //El thread nos dice el termino de la suma 
    n+=1;
    double x= xrange[i];

    result[threadIdx.x+tam*i]=(-20 * cos(n * PI))/(3 * pow(n,2)) * cos((n*x)); //Genera los terminos de la serie
    //printf("%f ",result[(n-1)*tam+i]);


}

__global__ void print_grid(double *matrix, int i, int j){
    for(int k=0; k<i; k++){
        for(int l=0; l<j; l++){
            printf("%f ",matrix[k*i+l]);
        }
        printf("\n");
    }
    printf("\n\n");
}


__global__ void def_funct(double *matrix, double* reconstructed_funct,int n_max){
    int n = threadIdx.x;
    double aux=0;
    for (int i = 0; i < n_max; i++)
    {
        aux+=matrix[n * n_max + i];//Reconstruye la señal paralelamente a partir de los terminos de la serie   
        }
      reconstructed_funct[n]+=aux;
}




// Arreglo uniformemente espaciado para los datos en x
double *linspace(double ini, double fin, double step, int* size){
    int aux=(int)((fin-ini)/step);
    //printf("aux: %d\n",aux);
    double *space=(double*)malloc(aux*sizeof(double)+1);
    double n_step=(fin-ini)/aux;
    if(space==NULL){
        perror("Not enough memory");
        exit(-1);
    }
    int i=0;
    space[i]=ini;
    //printf("%f, ",space[i]);
    for(int i=1; i<aux+1; i++){
        space[i]=space[i-1]+n_step;
        //printf("(%f %d), ",space[i],i);
    }
    *size=aux+1;
return space;

}

void write_buffer(double *space, double *vals,char * file, int space_size){
    FILE *f=fopen(file,"w");
    if(f==NULL){
        perror("File error");
        exit(-1);
    }
    for (int i = 0; i < space_size; i++) {
        if(i==space_size-1){
            fprintf(f, "%lf\n", space[i]);
        }else{
            fprintf(f, "%lf, ", space[i]);
        }
        
    }

    for (int i = 0; i < space_size; i++) {
        if(i==space_size-1){
            fprintf(f, "%lf\n", vals[i]);
        }else{
            fprintf(f, "%lf, ", vals[i]);
        }
        
    }
    
}