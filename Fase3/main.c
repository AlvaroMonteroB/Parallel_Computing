#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include<math.h>

struct sembuf sem_op;


#define PI 3.14159265359
#define a0 4-((10*pow(PI,2))/9)
void fork_series(int n_max);
double *linspace(double ini, double fin, double step,int *size);
double calc_term(int n, double x);
void inicializar_semaforo(int sem_id);
void bloquear_semaforo(int sem_id);
void desbloquear_semaforo(int sem_id);
void write_buffer(double *space, double *vals,char * file, int space_size);


//gcc main.c -o a -lm




int main(){
    int n_max=24;
    int tam;
    double *x_range=linspace(-3.14,3.14,.1,&tam);//Nuestro eje x
    
     key_t mem_key = ftok("main.c", 'c');
    //Crear segmento de memoria para el resultado
    int segment_id = shmget(mem_key, tam*sizeof(double), IPC_CREAT | S_IRUSR | S_IWUSR);
    double *reconstructed_funct=(double*)shmat(segment_id,NULL,0);

    //Crear semáforo para no sobreescribir cálculos
    key_t key=ftok("main.c", 'J');
    int sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("Error creando el semáforo");
        exit(1);
    }
    //Semaforo para asegurarnos que los procesos hijos han terminado
    key_t key_p=ftok("main.c",'k');
    int sem_p=semget(key_p,1,IPC_CREAT|0666);
    if (sem_p == -1) {
        perror("Error creando el semáforo");
        exit(1);
    }

    for (int i = 0; i < tam; i++)
    {   
        reconstructed_funct[i]=a0/2-2;
    }
    
    //Semaforo de la memoria compartida y de procesos

    inicializar_semaforo(sem_id);
    inicializar_semaforo(sem_p);


    for(int i = 0;  i < tam; i++){//Moviéndonos en x
        pid_t process[n_max];
        
        for(int n  = 1; n<=n_max; n++ ){//Moviéndonos por los n
            if((process[n-1]=fork())<0){
                perror("\nError al crear el proceso\n");
                exit(-1);
            }else if(process[n-1]==0){
                bloquear_semaforo(sem_p);
                double xn=calc_term(n,x_range[i]);//Calcular serie de fourier
                //bloquear semaforo
                bloquear_semaforo(sem_id);
                reconstructed_funct[i]+=xn;//Meter a memoria compartida los resultados

                // Desbloquear el semáforo después de modificar
                desbloquear_semaforo(sem_id);
                desbloquear_semaforo(sem_p);
                exit(0);
            }
        }
         for (int n = 0; n < n_max; n++) {
            wait(NULL);
        }
        
    }
    
    bloquear_semaforo(sem_p);
    desbloquear_semaforo(sem_p);
    printf("tam %d\n",tam);
    for (int i = 0; i < tam; i++) {
            if(i==tam-1){
                printf("(%lf,%d)\n", reconstructed_funct[i],i);
            }else{
                printf( "(%lf,%d), ", reconstructed_funct[i],i);
            }

            
        }

    
    write_buffer(x_range,reconstructed_funct,"result.csv",tam);
    shmdt(reconstructed_funct);
    shmctl(segment_id, IPC_RMID, NULL);
    
        
    
    system("ipcrm -a");
    semctl(sem_id, 0, IPC_RMID);
    semctl(sem_p, 0, IPC_RMID);



    return 0;
}

//Términos de la serie
double calc_term(int n, double x){
    double an = (-20 * cos(n * PI))/(3 * pow(n,2));
    return an * cos((n*x));
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

void inicializar_semaforo(int sem_id) {
    // Inicializar el semáforo a 1 (libre)
    semctl(sem_id, 0, SETVAL, 1);
}

void bloquear_semaforo(int sem_id) {
    sem_op.sem_num = 0;  // Índice del semáforo (si tuviéramos más de uno)
    sem_op.sem_op = -1;  // Decrementar (bloquear)
    sem_op.sem_flg = 0;  // Ninguna bandera especial

    // Ejecuta la operación de bloqueo
    semop(sem_id, &sem_op, 1);
}

void desbloquear_semaforo(int sem_id) {
    sem_op.sem_num = 0;  // Índice del semáforo
    sem_op.sem_op = 1;   // Incrementar (desbloquear)
    sem_op.sem_flg = 0;

    // Ejecuta la operación de desbloqueo
    semop(sem_id, &sem_op, 1);
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