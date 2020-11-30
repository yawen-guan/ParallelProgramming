#include <iostream>
#include <random>
#include <cstdio>
#include "common.hpp"

using namespace std;

const int MAX = (int)1e9;

void randomNumberGenerate(FILE* fout){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-MAX,MAX);

    for(int i = 0; i < VectorSize; i ++)
        fprintf(fout, "%d ", (int)dis(gen));
}

void makeData(){
    FILE* fv1 = fopen("vector1", "w");
    randomNumberGenerate(fv1);    

    FILE* fv2 = fopen("vector2", "w");
    randomNumberGenerate(fv2);
    
    fclose(fv1);
    fclose(fv2);
}