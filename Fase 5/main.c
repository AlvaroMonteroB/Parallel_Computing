#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14
#define a0 4 - ((10*pow(PI,2))/9)

double f(unsigned int n, double x);
double *linspace(double p0, double pf, double intervalo, int *tamanio);
void generar_archivo(double *x, double *y, char *nombre, int tamanio);

int main(int argc, char **argv)
{
    int word_rank;
    int id_proceso;
    int tamanio;
    double *dominio_x;
    double *y_reconstruida;
    double y;
    double suma = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &word_rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proceso);

    if(word_rank != 4)
        printf("Se requieren 8 procesos.\n");
    else
    {
        dominio_x = linspace(-PI, PI, 0.1f, &tamanio);
        y_reconstruida = malloc(sizeof(double) * tamanio);

            // Moviendose en X
            for(unsigned int i = 0; i < tamanio; i++)
            {
                // Moviendose en N por cada valor de x
                switch(id_proceso)
                {
                    case 0: y = a0/2-2 + f(8, dominio_x[i]); + f(16, dominio_x[i]);;
                        break;
                    case 1: y = f(1, dominio_x[i]) + f(9, dominio_x[i]); + f(17, dominio_x[i]);;
                        break;
                    case 2: y = f(2, dominio_x[i]) + f(10, dominio_x[i]); + f(18, dominio_x[i]);;
                        break;
                    case 3: y = f(3, dominio_x[i]) + f(11, dominio_x[i]); + f(19, dominio_x[i]);;
                        break;
                    /*
                    case 4: y = f(4, dominio_x[i]) + f(12, dominio_x[i]); + f(20, dominio_x[i]);;
                        break;
                    case 5: y = f(5, dominio_x[i]) + f(13, dominio_x[i]); + f(21, dominio_x[i]);;
                        break;
                    case 6: y = f(6, dominio_x[i]) + f(14, dominio_x[i]); + f(22, dominio_x[i]);;
                        break;
                    case 7: y = f(7, dominio_x[i]) + f(15, dominio_x[i]); + f(23, dominio_x[i]);;
                        break;*/
                }

                MPI_Reduce(&y, &suma, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

                if(id_proceso == 0)
                    printf("x = %f. y = %f.     \n", dominio_x[i], suma);

                y_reconstruida[i] = suma;
                //printf("%f\n", y_reconstruida[i]);
            }
        generar_archivo(dominio_x, y_reconstruida, "resultados.csv", tamanio);
    }


    MPI_Finalize();
    return 0;
}


// Función evaluada
double f(unsigned int n, double x)
{
    return cos(n*x) * ((-20*cos(PI*n)) / (3*pow(n, 2)));
}


double *linspace(double p0, double pf, double intervalo, int *tamanio)
{
    int aux=(int)((pf-p0)/intervalo);
    double *space = (double*)malloc(aux*sizeof(double)+1);
    double n_step = (pf-p0)/aux;

    if(space==NULL)
    {
        perror("Not enough memory");
        exit(-1);
    }

    space[0] = p0;
    for(int i=1; i < aux+1; i++)
        space[i]=space[i-1]+n_step;

    *tamanio = aux+1;
    return space;
}

void generar_archivo(double *x, double *y, char *nombre, int tamanio)
{
    FILE *f = fopen(nombre,"w");
    if(f == NULL) EXIT_FAILURE;

    for(int i = 0; i < tamanio; i++)
    {
        if(i==tamanio-1)
            fprintf(f, "%lf\n", x[i]);
        else
            fprintf(f, "%lf, ", x[i]);

    }

    for(int i = 0; i < tamanio; i++)
    {
        if(i==tamanio-1)
            fprintf(f, "%lf\n", y[i]);
        else
            fprintf(f, "%lf, ", y[i]);
    }

}


