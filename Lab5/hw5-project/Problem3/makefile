EXECS=mpi_hello_world mpi_bandwidth mpi_latency

MPICC=mpicc

all: ${EXECS}

mpi_hello_world: mpi_hello_world.c
	$(MPICC) -o mpi_hello_world mpi_hello_world.c

mpi_bandwidth: mpi_bandwidth.c
	$(MPICC) -o mpi_bandwidth mpi_bandwidth.c

mpi_latency: mpi_latency.c 
	$(MPICC) -o mpi_latency mpi_latency.c

clean:
	rm $(EXECS)