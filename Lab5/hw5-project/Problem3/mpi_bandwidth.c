#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#define NUMBER_OF_TESTS 10

int main(int argc, char **argv) {
    double *sbuf, *rbuf;
    int rank, n;
    double t1, t2, tmin;
    MPI_Status status, statuses[2];
    MPI_Request r[2];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
        printf("n\ttime (sec)\tRate (MB/sec)\n");

    for (n = 1000; n < 1100000; n *= 2) {
        sbuf = (double *)malloc(n * sizeof(double));
        rbuf = (double *)malloc(n * sizeof(double));
        if (!sbuf || !rbuf) {
            fprintf(stderr, "Could not allocate send/recv buffers of size %d\n", n);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        tmin = 1000;
        for (int k = 0; k < NUMBER_OF_TESTS; k++) {
            if (rank == 0) {
                /* Make sure both processes are ready */
                MPI_Sendrecv(MPI_BOTTOM, 0, MPI_INT, 1, 14,
                             MPI_BOTTOM, 0, MPI_INT, 1, 14, MPI_COMM_WORLD, &status);
                t1 = MPI_Wtime();
                MPI_Isend(sbuf, n, MPI_DOUBLE, 1, k, MPI_COMM_WORLD, &r[0]);
                MPI_Irecv(rbuf, n, MPI_DOUBLE, 1, k, MPI_COMM_WORLD, &r[1]);
                MPI_Waitall(2, r, statuses);
                t2 = MPI_Wtime() - t1;
                if (t2 < tmin) tmin = t2;
            } else if (rank == 1) {
                /* Make sure both processes are ready */
                MPI_Sendrecv(MPI_BOTTOM, 0, MPI_INT, 0, 14,
                             MPI_BOTTOM, 0, MPI_INT, 0, 14, MPI_COMM_WORLD, &status);
                MPI_Irecv(rbuf, n, MPI_DOUBLE, 0, k, MPI_COMM_WORLD, &r[0]);
                MPI_Isend(sbuf, n, MPI_DOUBLE, 0, k, MPI_COMM_WORLD, &r[1]);
                MPI_Waitall(2, r, statuses);
            }
        }

        if (rank == 0) {
            double rate;
            if (tmin > 0)
                rate = 2 * n * sizeof(double) * 1.0e-6 / tmin;
            else
                rate = 0.0;
            printf("%d\t%f\t%f\n", n, tmin, rate);
        }
        free(sbuf);
        free(rbuf);
    }

    MPI_Finalize();
    return 0;
}