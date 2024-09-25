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
    int rows=3,cols=9;
    write_matrix(matrix);
    FILE *f=read_matrix("matrix.txt");// inheritance to child process
    FIle *result=fopen("Result.txt",'w');
    pid_t process=fork();
    if (process==0){
        printf("Hola desde el proceso %d\n",getppid());
    }
    
    
    

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
    FILE *f = fopen("matrix.txt","r");
    if(f==NULL){
        exit(-1);
    }
    return f;
}

