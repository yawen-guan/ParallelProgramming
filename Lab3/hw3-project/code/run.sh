clang -fsanitize=thread -g -O1 multithread.c -o multithread

./multithread >result.txt  2>&1


clang multithread.c -emit-llvm -c -o multithread.bc

clang multithread.c -emit-llvm -S -c -o multithread.ll