//Montero Barraza Álvaro David
//Villeda Patricio Arón Alejandro
//Calderon Sabbagh Juan Alberto
//Olicon Alvarez Jessica Guadalupe


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>

typedef struct{
    int rows;
    int cols;
    int *data;
}Matrix_int;


int write_matrix(int matrix[3][9]);
FILE *read_matrix(char * stream);
void write_to_file(FILE *f,FILE *result,int i);


int main(){

    int matrix[3][9]={{1,2,3,4,5,6,7,8,9},{1,3,5,7,9,11,13,15,17},{2,4,6,8,10,12,14,16,18}};
    //int rows=3,cols=9;
    write_matrix(matrix);
    
    FILE *result=fopen("Result.txt","w");
    pid_t process[9];

        process[0]=fork();
        if(process[0]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[0]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,0);
            exit(0);
        }

        process[1]=fork();
        if(process[0]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[1]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,1);
            exit(0);
        }

        process[2]=fork();
        if(process[0]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[2]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,2);
            exit(0);
        }

        process[3]=fork();
        if(process[2]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[3]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,3);
            exit(0);
        }

        process[4]=fork();
        if(process[0]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[4]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,4);
            exit(0);
        }

        process[5]=fork();
        if(process[0]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[5]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,5);
            exit(0);
        }


        process[6]=fork();
        if(process[0]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[6]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,6);
            exit(0);
        }

        process[7]=fork();
        if(process[0]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[7]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,7);
            exit(0);
        }


        process[8]=fork();
        if(process[5]==-1){          
            perror("\nError al crear el proceso\n");
            exit(-1);
            }
        if (process[8]==0){
            FILE *f=read_matrix("matrix.txt");
            write_to_file(f,result,8);
            exit(0);
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
            printf("Hola desde el proceso %d\n",getppid());
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
            int result_value=0;
            for (int i=0;i<3;i++){
                result_value+=vector[i];
            }

            fprintf(result,"%i ",result_value);
            fflush(result);
            fclose(f);
            sleep(.001);
            
}