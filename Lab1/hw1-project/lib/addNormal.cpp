#include <immintrin.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "common.hpp"

using namespace std;

int v1[VectorSize], v2[VectorSize], ans_vec[VectorSize];

void addNormal(){
    FILE* fv1 = fopen("vector1", "r");
    FILE* fv2 = fopen("vector2", "r");
    FILE* fout = fopen("addNormal_output", "w");

    for(int i = 0; i < VectorSize; i ++){
        fscanf(fv1, "%d", &v1[i]);
        fscanf(fv2, "%d", &v2[i]);
    }

    auto start = chrono::system_clock::now();
    for(int i = 0; i < VectorSize; i ++)
        for (int times = 0; times < TIMES; times ++)
            ans_vec[i] = v1[i] + v2[i];
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    for(int i = 0; i < VectorSize; i ++)
        fprintf(fout, "%d ", ans_vec[i]);

    fclose(fv1);
    fclose(fv2);
    fclose(fout);
}