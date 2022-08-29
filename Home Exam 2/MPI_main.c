#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "functions.h"


int main(int argc, char *argv[]){
  int M=0, N=0, K1=0, K2=0;
  int my_rank, numprocs;
  float *inpic=NULL, *out_temp=NULL, *outpic=NULL;
  float *kern1=NULL, *kern2=NULL;

  MPI_Init (&argc, &argv); //initializing the MPI parallel region
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); //getting the process ID
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs); //getting the total no. of processes

  if (my_rank == 0) {
    srand(5);
    //read from command line the values of M, N, K1 and K2
    if (argc > 4){
      M = atoi(argv[1]); //no. of rows
      N = atoi(argv[2]); //no. of columns
      K1 = atoi(argv[3]); //size of kernel1
      K2 = atoi(argv[4]); //size of kernel 2
    } else {
      printf("Wrong usage! The command line arguments are\n");
      printf("mpirun -np <tot_proc> <M_rows> <N_col> <K1> <K2>\n");

    }
    //allocate the input matrix with M rows and N columns, with size MxN
    inpic = malloc(M*N * sizeof *inpic);

    //allocate the temporary output matrix with size (M-K+1)x(N-K+1). This is
    //used to store the output of the single convolution, to be used as input
    //for the second convolution
    out_temp = malloc(((M-K1+1)*(N-K1+1)) * sizeof *out_temp);
    //allocating the output matrix with size (M-K1-K2+2)x(N-K1-K2+2). This will
    //contain the result after double layer convolution.
    outpic = malloc(((M-K1-K2+2)*(N-K1-K2+2))  * sizeof *outpic);

    //allocate the convolutional kernel, kernel 1, with K1 rows and K1 columns
    kern1 = malloc(K1*K1 * sizeof *kern1);
    //allocate the convolutional kernel, kernel 2, with K2 rows and K2 columns
    kern2 = malloc(K2*K2 * sizeof *kern2);

    //filling the input matrix with some random values
    for(int i=0; i<M ;i++){
      for(int j=0; j<N; j++){
          inpic[i*N +j] = generate_random(0, 255);
      }
    }

    //filling kernnel 1 with some values
    for(int i=0; i<K1 ;i++){
      for(int j=0; j<K1; j++){
          kern1[i*K1 +j] = generate_random(-5,5);
      }
    }

    //filling kernel 2 with some values
    for(int i=0; i<K2 ;i++){
      for(int j=0; j<K2; j++){
          kern2[i*K2 +j] = generate_random(-7,7);
      }
    }
    printf("\n");
    printf("The input values for the parameters are\n");
    printf("---------------------------------------------\n");
    printf("M(rows)=%d, N(columns)=%d, K1=%d and K2=%d on rank=%d\n",M,N,K1,K2,my_rank);

    }//rank 0 end

    //process 0 broadcasts values of M, N, K1, and K2 to all the other processes
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K2, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank > 0) {
      //allocate the convolution kernels, kern1 and kern2 on all the other processes.
      //kern1 is of size K1xK1, and kern2 is if size K2xK2
      kern1 = malloc(K1*K1 * sizeof *kern1);
      kern2 = malloc(K2*K2 * sizeof *kern2);
    }

    //process 0 broadcasts the content of kernel 1 and kernel 2 to all the other processes
    MPI_Bcast(kern1, (K1*K1), MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(kern2, (K2*K2), MPI_FLOAT, 0, MPI_COMM_WORLD);

    //parallel computation of a double-layer convolution
    MPI_double_layer_convolution(M, N, inpic, K1, K2, kern1, kern2, out_temp, outpic);
    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank==0) {
      printf("\n");
      printf("Checking the computations on rank 0\n");
      printf("----------------------------------------\n");

      //allocating arrays to store the results of serial convolution
      float *out_temp_check = malloc(((M-K1+1)*(N-K1+1)) * sizeof *out_temp_check);
      float *outpic_check = malloc(((M-K1-K2+2)*(N-K1-K2+2))  * sizeof *outpic_check);

      //serial execution of a double layer convolution
      single_layer_convolution(M, N, inpic, K1, kern1, out_temp_check);
      single_layer_convolution((M-K1+1), (N-K1+1), out_temp_check, K2, kern2, outpic_check);

      //double sum_err_sqr(float* y, float* y2, int N)
      double err = sum_err_sqr(outpic, outpic_check, ((M-K1-K2+2)*(N-K1-K2+2)));
      printf("The error in the output matrix, from the \n");
      printf("serial and parallel execution is\n");
      printf("Error  =  %f\n",err);
      printf("----------------------------------------\n");

      free(out_temp_check);
      free(outpic_check);

    }

    free(inpic);
    free(out_temp);
    free(outpic);
    free(kern1);
    free(kern2);

    MPI_Finalize(); //end of parallel region

      return 0;
    }
