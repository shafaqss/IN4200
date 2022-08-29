#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void MPI_double_layer_convolution(int M, int N, float *input, int K1, int K2, float *kernel1, float *kernel2, float *output_temp, float *output);
void MPI_single_layer_convolution(int M, int N, float *input, int K, float *kernel, float *output);

//function to print a 1D vector of type int
void print_1D(int *vec, int S){
  printf("\n");
  printf("[");
  for (int i=0; i<S; i++){
    printf("%d, ", vec[i]);
  }
  printf("]");
  printf("\n");
}

//function to find the error between two 1D arrays, used for checking results
//of the output matrices produced from parallel and serial convolution
double sum_err_sqr(float* y, float* y2, int N){
    double error = 0;
    for (size_t i = 0; i < N; i++) {
        error += (y[i] - y2[i])*(y[i] - y2[i]);
    }
    return error;
}

//function that generates random numbers in the range [lower,upper]
//used for filling the input matrices for convolution
int generate_random(int lower, int upper){
  int num = (rand() % (upper - lower + 1)) + lower;
  return num;
}

//function to print out a 1D matrix of type float
int print_matrix_1D(float* A, int M, int N){
  //M is the row length, N is the column length
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            printf("%7.2lf ", A[i*N + j]);
        }
        printf("\n");
    }
    return 0;
}

//function to print out a 2D matrix of type float
int print_matrix_2D(float** A, int M, int N){
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            printf("%7.4lf ", A[i][j]);
        }
        printf("\n");
    }
    return 0;
}

//serial function for single layer convolution, used for checking results in the end
void single_layer_convolution(int M, int N, float *input, int K, float *kernel, float *output){

  double temp;
  for (int i=0; i<=(M-K) ; i++){
    for (int j=0; j<=(N-K) ; j++){

      temp = 0.0;

      for (int ii=0; ii<K ; ii++){
        for(int jj=0; jj<K ; jj++){

          temp += input[(i+ii)*N + (j+jj)] * kernel[(ii*K) + jj];
        } //printf("the temp is %f\n",temp);

      }
    output[(i*(N-K+1)) + j] = temp;
    }
  }
}//end of func

#endif
