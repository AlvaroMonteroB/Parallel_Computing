#include<stdio.h>
#include<mpi.h>

//mpicc main.c -o a 
int main(argc, argv)
int argc;
char **argv;
{
   int procesos;
   int identificador;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &procesos);
   MPI_Comm_rank(MPI_COMM_WORLD, &identificador);
   
   printf("Hola mundo desde el proceso %d de %d \n", identificador, procesos);
   
   MPI_Finalize();
   return 0;
}

