CC = gcc

main: mmio.c mmio.h main.c 
	$(CC) -fopenmp -O0 mmio.c main.c -o $@

.PHONY : clean
clean :
	rm main