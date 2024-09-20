#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>



int s_process_check();
int write_matrix();
int *read_matrix(char * stream,int col);



int main(){
    int rows=3,cols=9;
    write_matrix();
    pid_t process[9];
    for (int i = 0; i < cols; i++)
    {
        process[i]=fork();
        s_process_check(process[i]);
        
    }
    
    

    return 0;
}



int write_matrix(){
    int matrix[3][9]={{1,2,3,4,5,6,7,8,9},{1,3,5,7,9,11,13,15,17},{2,4,6,8,10,12,14,16,18}};
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

int *read_matrix(char *stream,int col){
    FILE *f = fopen("matrix.txt","r");
    if(f==NULL){
        exit(-1);
    }

}

int s_process_check(pid_t son){
    if (son<0){
        exit(-1);
    }
    else{
        return son;
    }
}