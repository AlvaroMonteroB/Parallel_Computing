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
#include <wait.h>
#include <sys/sem.h>

#define PERMISO 0777

int crear_semaforo(key_t llave, int valor_inicial);
void verde(int semid);  // down
void rojo(int semid);  // up
void regionCritica();

// Proceso
int main(void)
{
    // Variables
    int *matriz_mc;
    int *resultados;

    // Creación del semáforo
    key_t llave = ftok("semaforo", 'k');
    int semaforo = crear_semaforo(llave, 1);  // Inicia en rojo

    // Ejecutando proceso
    verde(semaforo);
        regionCritica();    // Ejecutando la tarea del proceso
    rojo(semaforo);
    return 0;
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


// Tarea del proceso
void regionCritica()
{
    // Variables
    unsigned short int fila;
    unsigned int producto = 1;
    int (*matriz_mc)[9];
    int *resultados;

    // Accediendo a la memoría compartida de la matriz
    key_t llave_matriz = ftok("matriz", 'k');
    int shmid_matriz = shmget(llave_matriz, 3*9*sizeof(int), IPC_CREAT | PERMISO);
    if((matriz_mc = shmat(shmid_matriz, 0, 0)) == (void *)-1) perror("ERROR");
    shmdt(matriz_mc);  // Cierre del bloque de memoría compartida de matriz


    // Generando memoría compartida para los resultados
    key_t llave_resultados = ftok("resultados", 'k');
    int shmid_resultados = shmget(llave_resultados, 3*sizeof(int), IPC_CREAT | PERMISO);
    if((resultados = (int *)shmat(shmid_resultados, 0, 0)) == (void *)-1) perror("ERROR");


    // Determinando con qué fila trabajará el proceso
    if(resultados[27] == 0)
        fila = 0;
    else if(resultados[28] == 0)
        fila = 1;
    else if(resultados[29] == 0)
        fila = 2;

    for(unsigned short int j = 0; j < 9; j++)
        printf("%i ", matriz_mc[fila][j]);
    printf("\n");

    // Fila correspondiente
    printf("Proceso %i multiplicando los valores de la fila %i\n", getpid(), fila);
    for(unsigned short int j = 0; j < 9; j++)
        producto *= matriz_mc[fila][j];

    // Registrando resultado
    resultados[27+fila] = producto;
    printf("%i\n", producto);
    printf("Resultado: %i\n", resultados[fila+27]);

    // Cerrando bloques de memoría compartida
    shmdt(resultados);

    sleep(2);
}


