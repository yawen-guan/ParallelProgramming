#include <immintrin.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <pthread.h>
#include "common.hpp"

using namespace std;

int vec1[VectorSize], vec2[VectorSize], ans[VectorSize];

void addUnit(int st, int ed){
    for(int i = st; i < ed; i ++)
        for(int times = 0; times < TIMES; times ++) 
            ans[i] = vec1[i] + vec2[i];
}

void addUsingThreads(){
    FILE* fv1 = fopen("vector1", "r");
    FILE* fv2 = fopen("vector2", "r");
    FILE* fout = fopen("addUsingThreads_output", "w");

    for(int i = 0; i < VectorSize; i ++){
        fscanf(fv1, "%d", &vec1[i]);
        fscanf(fv2, "%d", &vec2[i]);
    }

    const int len = VectorSize / 4;
    thread *t[4];
    auto start = chrono::system_clock::now();
    for(int i = 0; i < 4; i ++)
        t[i] = new thread{addUnit, i * len, (i + 1) * len};
    for(int i = 0; i < 4; i ++)
        t[i] -> join();
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::cout <<  "elapsed time: " << elapsed_seconds.count() << "s\n";

    for(int i = 0; i < VectorSize; i ++)
        fprintf(fout, "%d ", ans[i]);
    fclose(fv1);
    fclose(fv2);
    fclose(fout);
}
