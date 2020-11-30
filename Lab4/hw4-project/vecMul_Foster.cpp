#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <random>
using namespace std;

const int MAX = (int)1e9;
const int SIZE = 1000;

int a[SIZE][SIZE], b[SIZE], c[SIZE];

void init()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-MAX, MAX);

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            a[i][j] = (int)dis(gen);

    for (int i = 0; i < SIZE; i++)
        b[i] = (int)dis(gen);

    memset(c, 0, sizeof(c));
}

void mulUnit(int st, int ed)
{
    for (int i = st; i < ed; i++)
        for (int j = 0; j < SIZE; j++)
            c[i] = a[i][j] * b[j];
}

int main()
{
    init();

    thread *t[6];
    int firstRow = 0;
    for (int i = 0; i < 6; i++)
    {
        int len = (i < 4) ? 167 : 166;
        t[i] = new thread{mulUnit, firstRow, firstRow + len};
        firstRow += len;
    }
    for (int i = 0; i < 6; i++)
    {
        t[i]->join();
    }
    printf("finished.\n");

    return 0;
}