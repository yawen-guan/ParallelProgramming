#include <immintrin.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "common.hpp"

using namespace std;

const int N = VectorSize / 8;
int *aligned_int1[N], *aligned_int2[N];
__m256i int_vec1[N], int_vec2[N], ans_int[N];

void addUsingAVX(){
    FILE* fv1 = fopen("vector1", "r");
    FILE* fv2 = fopen("vector2", "r");
    FILE* fout = fopen("addUsingAVX_output", "w");

    for(int i = 0; i < VectorSize / 8; i ++){
        aligned_int1[i] = (int*) aligned_alloc(32, 8 * 8 * sizeof(int));
        aligned_int2[i] = (int*) aligned_alloc(32, 8 * 8 * sizeof(int));
        for(int j = 0; j < 8; j ++){
            fscanf(fv1, "%d", &aligned_int1[i][j]);
            fscanf(fv2, "%d", &aligned_int2[i][j]);
        }
        int_vec1[i] = _mm256_load_si256((const __m256i*) aligned_int1[i]);
        int_vec2[i] = _mm256_load_si256((const __m256i*) aligned_int2[i]);
    }

    auto start = chrono::system_clock::now();
    for(int i = 0; i < VectorSize / 8; i ++)
        for(int times = 0; times < TIMES; times ++)
             ans_int[i] = _mm256_add_epi32(int_vec1[i], int_vec2[i]);
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    for(int i = 0; i < N; i ++)
        for(int j = 0; j < 8; j ++){
            int *p = (int*)&ans_int[i];
            fprintf(fout, "%d ", p[j]);
        }

    fclose(fv1);
    fclose(fv2);
    fclose(fout);
}