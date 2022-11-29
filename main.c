// second largest element
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define World MPI_COMM_WORLD
#define Ignore MPI_STATUS_IGNORE
int main(int argc, char **argv) {
    int rank, size;
    int *A; //vector
    int *Aw, *second_arr;
    int N; // size of A
    int i, part;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(World, &size);
    MPI_Comm_rank(World, &rank);



    if (rank == 0) {
        printf("size of vector:\n");
        scanf("%d", &N);
        A = malloc(N * sizeof(int));
        for (i=0; i<N; i++) {
            A[i] = rand() % 1000;
            // printf("%3d", A[i]);
        }
        //printf("\n\n");

    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    part = N / size;

    if (rank == 0) {
        for (i=1; i<size; i++)
            MPI_Send(&A[i*part], part, MPI_INT, i, 0, MPI_COMM_WORLD);
    }


    else {
        Aw = malloc(part * sizeof(int));

        MPI_Recv(Aw, part, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int max = Aw[0];
        int second;

        for (i = 1; i<part; i++) {
            if (Aw[i] >= max) {
                second = max;
                max = Aw[i];
            } else if (Aw[i] > second) {
                second = Aw[i];
            }
        }
        
        MPI_Send(&second, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        second_arr = malloc(size*sizeof(int));
        for (i=1; i<size; i++)
            MPI_Recv(&second_arr[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // additional work start
        int max = A[(N/size)*size];
        int second;

        for (i=(N/size)*size; i<N; i++) {
            if (A[i] >= max) {
                second = max;
                max = A[i];
            } else if (A[i] > second) {
                second = A[i];
            }
        }
        second_arr[0] = second;
        // additional work finished 


        max = second_arr[0];
        int answer;
        for (i = 1; i<part; i++) {
            if (second_arr[i] >= max) {
                answer = max;
                max = second_arr[i];
            } else if (second_arr[i] > answer) {
                answer = second_arr[i];
            }
        }
        printf("\n\nSecond largest element is %d\n\n", answer);
    }
    MPI_Finalize();
    return 0;
}