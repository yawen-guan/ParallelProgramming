#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "mpi.h"
#define NUMBER_REPS 1000

int main(int argc, char *argv[]) {
    int reps,         /* number of samples per test */
        tag,          /* MPI message tag parameter */
        numtasks,     /* number of MPI tasks */
        rank,         /* my MPI task number */
        dest, source, /* send/receive task designators */
        avgT,         /* average time per rep in microseconds */
        rc,           /* return code */
        n;
    double T1, T2, sumT, deltaT;
    char msg;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);

    sumT = 0;
    msg = 'x';
    tag = 1;
    reps = NUMBER_REPS;

    if (rank == 0) {
        dest = 1;
        source = 1;
        for (n = 1; n <= reps; n++) {
            T1 = MPI_Wtime();
            rc = MPI_Send(&msg, 1, MPI_BYTE, dest, tag, MPI_COMM_WORLD);
            if (rc != MPI_SUCCESS) {
                printf("Send error in task 0!\n");
                MPI_Abort(MPI_COMM_WORLD, rc);
                exit(1);
            }
            rc = MPI_Recv(&msg, 1, MPI_BYTE, source, tag, MPI_COMM_WORLD, &status);
            if (rc != MPI_SUCCESS) {
                printf("Receive error in task 0!\n");
                MPI_Abort(MPI_COMM_WORLD, rc);
                exit(1);
            }
            T2 = MPI_Wtime();

            deltaT = T2 - T1;
            // printf("%4d  %8.8f  %8.8f  %2.8f\n", n, T1, T2, deltaT * 1000000);
            sumT += deltaT;
        }
        avgT = (sumT * 1000000) / reps;
        // printf("***************************************************\n");
        printf("\nAvg round trip time = %d microseconds\n", avgT);
        printf("Avg one way latency = %d microseconds\n", avgT / 2);
    }

    else if (rank == 1) {
        dest = 0;
        source = 0;
        for (n = 1; n <= reps; n++) {
            rc = MPI_Recv(&msg, 1, MPI_BYTE, source, tag, MPI_COMM_WORLD, &status);
            if (rc != MPI_SUCCESS) {
                printf("Receive error in task 1!\n");
                MPI_Abort(MPI_COMM_WORLD, rc);
                exit(1);
            }
            rc = MPI_Send(&msg, 1, MPI_BYTE, dest, tag, MPI_COMM_WORLD);
            if (rc != MPI_SUCCESS) {
                printf("Send error in task 1!\n");
                MPI_Abort(MPI_COMM_WORLD, rc);
                exit(1);
            }
        }
    }

    MPI_Finalize();
    exit(0);
}