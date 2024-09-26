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



int main(){

    int matrix[3][9]={{1,2,3,4,5,6,7,8,9},{1,3,5,7,9,11,13,15,17},{2,4,6,8,10,12,14,16,18}};
    //int rows=3,cols=9;
    write_matrix(matrix);
    
    FILE *result=fopen("Result.txt","w");
    pid_t process[9];
    for(int i=0;i<9;i++){
        process[i]=fork();
        if (process[i]>=0){
            FILE *f=read_matrix("matrix.txt");

            printf("%i aaa   ",process[i]);
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

            fprintf(result,"%i",result_value);
            fflush(result);
            fclose(f);
            sleep(.001);
            exit(0);
        }else{
            sleep(.001);
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
