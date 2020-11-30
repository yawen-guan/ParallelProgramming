#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mmio.h"

int main(int argc, char *argv[]) {
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;
    int *I, *J;
    double *val;
    int threadNum = 6;

    /**** matrix readin ****/
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [martix-market-filename] [thread number]\n", argv[0]);
        exit(1);
    } else {
        threadNum = atoi(argv[2]);
        if ((f = fopen(argv[1], "r")) == NULL)
            exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0) {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode)) {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0)
        exit(1);

    I = (int *)malloc(nz * sizeof(int));
    J = (int *)malloc(nz * sizeof(int));
    val = (double *)malloc(nz * sizeof(double));

    for (int i = 0; i < nz; i++) {
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--; /* adjust from 1-based to 0-based */
        J[i]--;
    }

    if (f != stdin) fclose(f);

    mm_write_banner(stdout, matcode);
    mm_write_mtx_crd_size(stdout, M, N, nz);
    // for (int i = 0; i < nz; i++)
    //     fprintf(stdout, "%d %d %20.19g\n", I[i] + 1, J[i] + 1, val[i]);

    /**** vector and ans ****/
    int *vector = (int *)malloc(N * sizeof(int));
    double *ans = (double *)malloc(M * sizeof(double));
    srand(0);
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 1000;
    }
    for (int i = 0; i < M; i++) {
        ans[i] = 0;
    }
    // /**** partition ****/
    // int st[10], ed[10], threadNum = 6;
    // for (int i = 0; i < threadNum; i++) {
    //     st[i] = (i == 0) ? 0 : ed[i - 1];
    //     ed[i] = (i == threadNum - 1) ? nz : st[i] + (nz / threadNum);
    // }
    // for (int p = 0; p < threadNum; p++) {
    //     for (int i = st[p]; i < ed[p]; i++) {
    //         ans[I[i]] = val[i] * vector[J[i]];
    //     }
    // }

    /**** mul ****/
    clock_t start, end;
    start = clock();
#pragma omp parallel for num_threads(threadNum) schedule(guided)
    for (int i = 0; i < nz; i++) {
        ans[I[i]] += val[i] * vector[J[i]];
    }
    end = clock();
    printf("time=%f\n", ((double)end - start) / (CLOCKS_PER_SEC / 1000));

    return 0;
}
