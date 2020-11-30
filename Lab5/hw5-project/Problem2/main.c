#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 1000

struct bufferStruct {
    int buf[SIZE];
    int head, tail, count;
} buffer;

void init() {
    buffer.head = buffer.tail = buffer.count = 0;
}

int isfull() {
    return (buffer.count == SIZE);
}

int isempty() {
    return (buffer.count == 0);
}

int put(int tag) {
    if (isfull()) return -1;
#pragma omp critical
    {
        buffer.buf[buffer.tail] = tag;
        buffer.tail = (buffer.tail + 1) % SIZE;
        buffer.count++;
    }
    return 1;
}

int get() {
    if (isempty()) return -1;
    int tag;
#pragma omp critical
    {
        tag = buffer.buf[buffer.head];
        buffer.head = (buffer.head + 1) % SIZE;
        buffer.count--;
    }
    return tag;
}

void producer(int st, int ed) {
    for (int i = st; i < ed; i++) {
        if (put(i) == -1)
            printf("Buffer is full. \n");
        else
            printf("Producing %d by thread %d\n", i, omp_get_thread_num());
        sleep(1);
    }
}

void consumer(int cnt) {
    for (int i = 0; i < cnt; i++) {
        int tag = get();
        if (tag == -1)
            printf("Buffer is empty. \n");
        else
            printf("Consuming %d by thread %d\n", tag, omp_get_thread_num());
        sleep(1);
    }
}

int main() {
    init();

#pragma omp parallel num_threads(6) 
    {
#pragma omp sections
        {
#pragma omp section
            producer(0, 10);
#pragma omp section
            producer(10, 20);
#pragma omp section
            producer(20, 30);
#pragma omp section
            consumer(10);
#pragma omp section
            consumer(10);
#pragma omp section
            consumer(10);
        }
    }

    return 0;
}