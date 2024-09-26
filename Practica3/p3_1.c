//Montero Barraza Álvaro David
//Villeda Patricio Arón Alejandro
//Calderon Sabbagh Juan Alberto
//Olicon Alvarez Jessica Guadalupe


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void write_to_file(FILE *f,FILE *result,int i);

int main(){




    return 0;
}




void write_to_file(FILE *f,FILE *result,int i){
    

            //printf("%i aaa   ",process[i]);
            //printf("Hola desde el proceso %d\n",getppid());
            int value;
            int vector[3];
            for (int k = 0; k < 3; k++) {//rows
                for (int j = 0; j < 9; j++) {//cols
                    fscanf(f, "%i", &value);
                        if (j == i) {
                            vector[k]=value;
                                    }
                    }
                }
            int result_value=1;
            for (int i=0;i<3;i++){
                result_value*=vector[i];
            }

            fprintf(result,"%i ",result_value);
            fflush(result);
            fclose(f);
            sleep(.001);
            
}