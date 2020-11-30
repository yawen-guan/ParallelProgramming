#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int Global;

const int constGlobal = 10;

const char constChar[] = "content";
 
void *Thread1(void *x) {
    Global++;
    return NULL;
}
 
void *Thread2(void *x) {
    Global--;
    return NULL;
}

void *FuncUsingGlobalConst(void *x){
    int tmp = constGlobal;
    char ch = constChar[0];
    return NULL;
}

void *FuncUsingStaticConst(void *x){
    const static int sta = 0;
    return NULL;
}

int FuncUsingGlobalVariable(void *x){
    int tmp = Global;
    return tmp;
}

int FuncUsingStaticVariable(void *x){
    static int sta = 0;
    int tmp = sta;
    return tmp;
}

void *FuncUsingNewSpace(void *x){
    int *p = malloc(sizeof(int));
    return NULL;
}

void *FuncCallNonReentrantFunc(void *x){
    Thread1(NULL);
    return NULL;
}

void *FuncCallReentrantFunc(void *x){
    FuncUsingGlobalConst(NULL);
    return NULL;
}

const char *FuncReturnGlobalConst(void *x){
    return constChar;
}

const char *FuncReturnStaticConst(void *x){
    const static char sta[] = "stacontent";
    return sta; 
}

int main() {
    pthread_t t[2];
    pthread_create(&t[0], NULL, Thread1, NULL);
    pthread_create(&t[1], NULL, Thread2, NULL);
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
}