#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include<math.h>

#define PI 3.14159265359
#define a0 4-((10*PI*PI)/9)
void fork_series(int n_max);
double *linspace(double ini, double fin, double step,int *size);

//gcc main.c -o a -lm

double calc_term(int n, double x);

int main(){
    int n_max;
    int tam;
    double *x_range=linspace(-3.14,3.14,.1,&tam);//Nuestro eje x
    
    //Crear segmento de memoria para el resultado
    int segment_id = shmget(IPC_PRIVATE, tam*sizeof(double), IPC_CREAT | S_IRUSR | S_IWUSR);
    double *reconstructed_funct=(double*)shmat(segment_id,NULL,0);


    for(int i = 0;  i < tam; i++){//Moviéndonos en x
        pid_t process[n_max];
        //Crear semáforo para que calcule x
        for(int n  = 1; n<=n_max; n++ ){//Moviéndonos por los n
            if((process[n]=fork())<0){
                perror("\nError al crear el proceso\n");
                exit(-1);
            }else if(process[n]==0){
                double xn=calc_term(n,x_range[i]);//Calcular serie de fourier
                reconstructed_funct[i]+=xn;//Meter a memoria compartida los resultados
                exit(0);
            }
        }
        //Semáforo en bajo para continuar
    }
        
    
    system("ipcrm -a");
    return 0;
}

//Términos de la serie
double calc_term(int n, double x){
    double an = (-20 * cos(n * PI))/(3 * pow(n,2));
    double result = a0/2 + an *cos((2*n*x));
    return result;
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