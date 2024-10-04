//Montero Barraza Álvaro David
//Villeda Patricio Arón Alejandro
//Calderon Sabbagh Juan Alberto
//Olicon Alvarez Jessica Guadalupe


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>

int write_matrix(int matrix[3][9]);
FILE *read_matrix(char * stream);
void write_to_file(FILE *f,FILE *result,int i);


int main(){

    int matrix[3][9]={{1,2,3,4,5,6,7,8,9},{1,3,5,7,9,11,13,15,17},{2,4,6,8,10,12,14,16,18}};
    //int rows=3,cols=9;
    write_matrix(matrix);
    
    FILE *result=fopen("Result.txt","w");

    pid_t process[9];
    for(int i=0;i<9;i++){

        if((process[i]=fork())<0){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        
        else if(process[i]==0){
        //printf("Hola desde el proceso %d\n",getppid());
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,i);
            exit(0);
        }
        
    }

    
    
    fclose(result);
    

    return 0;
}










int write_matrix(int matrix[3][9]){
    
    FILE *f=fopen("matrix.txt","w");
    if (f==NULL){
        printf("Error al abrir archivo\n");
        exit(-1);
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            char c[2];
            sprintf(c,"%i",matrix[i][j]);
            fprintf(f,"%s ",c);
        }
        fprintf(f,"\n");
        
    }
    fclose(f);
    
    return 0;
}

FILE *read_matrix(char *stream){
    FILE *f = fopen(stream,"r");
    if(f==NULL){
        exit(-1);
    }
    return f;
}

void write_to_file(FILE *f,FILE *result,int i){
    

            //printf("%i aaa   ",process[i]);
            
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
