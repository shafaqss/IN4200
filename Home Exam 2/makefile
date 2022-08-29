main_mpi: MPI_main.c MPI_double_layer_convolution.c
	/usr/lib64/openmpi/bin/mpicc MPI_main.c MPI_double_layer_convolution.c
	/usr/lib64/openmpi/bin/mpirun -np 15 ./a.out 1000 1000 4 13
