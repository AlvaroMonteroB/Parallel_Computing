#include <mpi.h>
#include <stdio.h>

// Funciones que realizan las operaciones según las indicaciones
int operation1(int a, int b, int c) {
    return a + b + c;
}

int operation2(int a, int b, int c) {
    return a - b + c;
}

int operation3(int a, int b, int c) {
    return a * b * c;
}

int operation4(int a, int b, int c) {
    return a * (b + c);
}

int operation5(int a, int b, int c) {
    return a * b - b * c;
}

int operation6(int a, int b, int c) {
    return c * (a + b) * (a + b);
}

int operation7(int a, int b, int c) {
    return a * a - b * b + c * c * c;
}

int main(int argc, char** argv) {
    int world_size, world_rank;
    int matriz[7][3] = {
        {1,  2,  3},
        {4,  5,  6},
        {7,  8,  9},
        {9,  6,  3},
        {-6, -9, -12},
        {5, 10, 15},
        {7, 14, 21}
    };

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size != 8) {
        if (world_rank == 0) {
            printf("Se requieren exactamente 8 procesos.\n");
        }
        MPI_Finalize();
        return 0;
    }

    if (world_rank == 0) {
        // Nodo 0: Envía una fila a cada uno de los nodos (del 1 al 7)
        for (int i = 1; i < world_size; i++) {
            MPI_Send(matriz[i-1], 3, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Nodo 0: Recibe los resultados de cada nodo
        int resultados[7];  // Arreglo para almacenar los resultados
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(&resultados[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("Nodo 0 recibió resultado %d del nodo %d\n", resultados[i-1], i);
        }

        // Imprimir el resultado final
        printf("\nResultado final:\n");
        for (int i = 0; i < 7; i++) {
            printf("Resultado de la operación en la fila %d: %d\n", i+1, resultados[i]);
        }

    } else {
        int fila[3]; // Arreglo para almacenar la fila recibida
        MPI_Recv(fila, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int a = fila[0], b = fila[1], c = fila[2];
        int resultado;

        // Cada nodo ejecuta la operación correspondiente
        switch (world_rank) {
            case 1:
                resultado = operation1(a, b, c);
                break;
            case 2:
                resultado = operation2(a, b, c);
                break;
            case 3:
                resultado = operation3(a, b, c);
                break;
            case 4:
                resultado = operation4(a, b, c);
                break;
            case 5:
                resultado = operation5(a, b, c);
                break;
            case 6:
                resultado = operation6(a, b, c);
                break;
            case 7:
                resultado = operation7(a, b, c);
                break;
        }

        // Enviar el resultado de vuelta al nodo 0
        MPI_Send(&resultado, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}