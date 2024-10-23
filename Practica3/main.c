/// PRÁCTICA 3. PROGRAMA DE CÓMPUTO PARALELO CON MEMORÍA COMPARTIDA Y SEMÁFOROS
/// CÓMPUTO PARALELO
/// 6BV1
/// Calderon Sabaggh Juan Alberto
/// Montero Barraza Alvaro David
/// Villeda Patricio Arón Alejandro

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#define PERMISO 0777

void guardar_matriz(int (*matriz_mc)[9], int matriz[3][9]);
int crear_semaforo(key_t llave, int valor_inicial);
void verde(int semid);  // down
void rojo(int semid);  // up

// Proceso coordinador
int main(void)
{
    // Variables
    int matriz[3][9] = {{1,2,3,4,5,6,7,8,9}, {1,3,5,7,9,11,13,15,17}, {2,4,6,8,10,12,14,16,18}};
    int (*matriz_mc)[9];
    int *resultados;

    // Creación del semáforo
    key_t llave = ftok("semaforo", 'k');
    int semaforo = crear_semaforo(llave, 1);  // Inicia en rojo

    // Se pone en verde el semáforo
    verde(semaforo);

        // Generando memoría compartida para la matriz
        key_t llave_matriz = ftok("matriz", 'k');
        int shmid_matriz = shmget(llave_matriz, 3*9*sizeof(int), IPC_CREAT | PERMISO);
        if((matriz_mc = shmat(shmid_matriz, 0, 0)) == (void *)-1) perror("ERROR");

        // Guardando los datos de la matriz en la memoría compartida
        guardar_matriz(matriz_mc, matriz);

        shmdt(matriz_mc);  // Cierre del bloque de memoría compartida de matriz

        // Generando memoría compartida para los resultados
        key_t llave_resultados = ftok("resultados", 'k');
        int shmid_resultados = shmget(llave_resultados, 3*sizeof(int), IPC_CREAT | PERMISO);
        if((resultados = (int *)shmat(shmid_resultados, 0, 0)) == (void *)-1) perror("ERROR");

        // Inicializando valores
        resultados[27] = 0;
        resultados[28] = 0;
        resultados[29] = 0;

    // Ponemos el semáforo en rojo
    rojo(semaforo);

    // Mostrando elementos guardados
    printf("MATRIZ EN LA MEMORÍA COMPARTIDA\n");
    for(unsigned short int i = 0; i < 3; i++)
    {
        for(unsigned short int j = 0; j < 9; j++)
            printf("%i ", matriz_mc[i][j]);
        printf("\n");
    }
    printf("\n");

    // Esperando los resultados
    while(resultados[27] == 0 || resultados[28] == 0 || resultados[29] == 0)
    {
        printf("Esperando resultados...\n");
        sleep(6);
    }

    // Resultados
    printf("\nRESULTADOS OBTENIDOS: \n");
    printf("[");
    for(unsigned short int i = 27; i < 30; i++)
        printf(" %i ", resultados[i]);
    printf("]");
    shmdt(&resultados);  // Cierre del bloque de memoría compartida de resultados

    // eliminando memoría compartida
    semctl(semaforo, 0, SETVAL, 0);  // Cierre del semáforo
    shmctl(shmid_matriz, IPC_RMID, 0);
    shmctl(shmid_resultados, IPC_RMID, 0);
    system("ipcrm -a");
    return 0;
}


// Guardamos en la memoría compartida la matriz
void guardar_matriz(int (*matriz_mc)[9], int matriz[3][9])
{
    for(unsigned short int i = 0; i < 3; i++)
        for(unsigned short int j = 0; j < 9; j++)
            matriz_mc[i][j] = matriz[i][j];
}


// Creamos un semáforo
int crear_semaforo(key_t llave, int valor_inicial)
{
    int semid = semget(llave, 1, IPC_CREAT | PERMISO);
    if(semid == -1) return -1;

    semctl(semid, 0, SETVAL, valor_inicial);
    return semid;
}


void verde(int semid)  // down
{
    struct sembuf op_p[] = {0, -1, 0};
    semop(semid, op_p, 1);
}


void rojo(int semid)  // up
{
    struct sembuf op_v[] = {0, +1, 0};
    semop(semid, op_v, 1);
}


