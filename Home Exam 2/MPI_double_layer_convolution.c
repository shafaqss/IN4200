#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

/*
Function: MPI_single_layer_convolution
--------------------------------
This function performs a single layer convolution on the input matrix,
with kernel, with MPI parallelization.

 int M: no. of rows in input array(matrix)
 int N: no. of columns in input array(matrix)
 float *input: input matrix to do convolution on
 int K: size of kernel (kernel has size KxK)
 float *kernel: convolutional kernel
 float *output: matrix that stores the results of the convolution, this has
                size (M-K+1)x(N-K+1)
*/
void MPI_single_layer_convolution(int M, int N, float *input, int K, float *kernel, float *output){
  int my_rank, numprocs;
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  //calculate the number of rows for each process and, find out the number of
  //rows with overlapp for each process
  int *n_rows = malloc(numprocs*sizeof *n_rows);
  int *n_rows_pad = malloc(numprocs*sizeof *n_rows_pad);

  //used when scattering the input matrix
  int *sendcounts = malloc(numprocs*sizeof *sendcounts);
  int *Sdispls = malloc(numprocs*sizeof *Sdispls);

  //used when gathering the output matrix
  int *recievecounts = malloc(numprocs*sizeof *recievecounts);
  int *Gdispls = malloc(numprocs*sizeof *Gdispls);
  Sdispls[0] = 0; Gdispls[0] = 0;

  int pad = K-1; //find how much padding is needed for each process
  int out = (M-K+1); //distribute the output matrix among the processes
  int rows = out/numprocs;
  int remainder = out%numprocs;

  //last remainder processes gets an extra row.
  for (int rank = 0; rank < numprocs-1; rank++) { //filling n_rows array
    n_rows[rank] = rows + ((rank >= (numprocs - remainder)) ? 1:0);
  }
  n_rows[numprocs-1] = rows + ((numprocs-1) >= (numprocs - remainder) ? 1:0);

  for (int r=0; r<numprocs; r++){ //filling n_rows_pad array
    if (n_rows[r] != 0) {
      n_rows_pad[r] = n_rows[r]+pad;
    } else {
      n_rows_pad[r] = 0;
    }
  }

  for (int r=0; r<numprocs; r++){  //filling sendcounts and recievecounts array
    sendcounts[r] = n_rows_pad[r]*N;
    recievecounts[r] = n_rows[r]*(N-K+1);
  }

  for (int r=0; r<(numprocs-1); r++){ //filling the Sdispls and Gdispls array
    Sdispls[r+1] = Sdispls[r] + (n_rows[r]*N);
    Gdispls[r+1] = Gdispls[r] + recievecounts[r];
  }

  //allocating the input matrix on all processes besides process 0 (this was
  //already done in the main function)
  if (my_rank > 0){
    input = malloc(N*n_rows_pad[my_rank] * sizeof *input);
  }

  //scattering the input matrix appropriately among all the other processes
  MPI_Scatterv(input,               //matters only for root process.
              sendcounts,           //matters only for root process.
              Sdispls,              //matters only for root process.
              MPI_FLOAT,
              input,                 //recieve buffer
              N*n_rows_pad[my_rank],
              MPI_FLOAT,
              0,
              MPI_COMM_WORLD);

  //allocating the output matrix on all processes besides process 0, to store
  //the convolution results on
  if (my_rank > 0){
    output = malloc(recievecounts[my_rank] * sizeof *output);
  }

  //this is the actual convolution calculation
  double temp;
  for (int i=0; i<(n_rows[my_rank]) ; i++){
    for (int j=0; j<=(N-K) ; j++){

      temp = 0.0;

      for (int ii=0 ; ii<K ; ii++){
        for(int jj=0  ; jj<K ; jj++){

          temp += input[(i+ii)*N + (j+jj)] * kernel[(ii*K) + jj];

        }
      }
      output[(i*(N-K+1)) + j] = temp;
    }
  }

  //gathering the output matrix from all processes on root process(rank 0)
  MPI_Gatherv(output,
              recievecounts[my_rank],
              MPI_FLOAT,
              output,              //matters only for root process
              recievecounts,
              Gdispls,
              MPI_FLOAT,
              0,
              MPI_COMM_WORLD);

free(n_rows);
free(n_rows_pad);
free(sendcounts);
free(Sdispls);
free(recievecounts);
free(Gdispls);

}//end of func

/*
Function: MPI_double_layer_convolution
--------------------------------
This function performs a double layer convolution on the input matrix, with kernel1
and then with kernel2, with MPI parallelization.

 int M: no. of rows in input array(matrix)
 int N: no. of columns in input array(matrix)
 float *input: input matrix to do convolution on
 int K1: size of kernel 1 (kernel 1 has size K1xK1)
 int K2: size of kernel 2 (kernel 2 has size K2xK2)
 float *kernel1: convolutional kernel 1
 float *kernel2: convolutional kernel 2
 float *output_temp: matrix to store the output after one convolution, this has
                    size (M-K1+1)x(N-K1+1), this will input for second convolution
 float *output: output matrix after double convolution, this has size (M-K1-K2+2)x(N-K1-K2+2)
*/
void MPI_double_layer_convolution(int M, int N, float *input, int K1, int K2, float *kernel1, float *kernel2, float *output_temp, float *output){
  MPI_single_layer_convolution(M, N, input, K1, kernel1, output_temp);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_single_layer_convolution((M-K1+1), (N-K1+1), output_temp, K2, kernel2, output);

}//end of func
