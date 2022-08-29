#include <stdio.h> //printf
#include <stdlib.h> //malloc and calloc
#include <time.h> //timing
#include <omp.h> //openMP

#include "functions.h"

int main(int argc, char *argv[]) {
  double timeused;

  #if defined(_OPENMP)
  {
    double start, end;

    int N; //total number of nodes
    char *filename1 = argv[1];
    char **table;
    int **SNN_matrix;

    printf("\n");
    printf("Timings for connectivity graph in 2D table format\n");
    printf("(with parallellization)\n");
    printf("\n");

    start = omp_get_wtime();
    read_graph_from_file1(filename1, &N, &table);
    end = omp_get_wtime();
    timeused = end-start;
    printf("Time taken for read_graph_from_file1 = %lf seconds\n", timeused);
    printf("----------------------------------------------------------\n");
    //printmat_char(table, N, N);

    omp_set_num_threads(11);   //OpenMP threads
    start = omp_get_wtime();
    create_SNN_graph1(N, table, &SNN_matrix);
    end = omp_get_wtime();
    timeused = end-start;
    printf("---------------------------------------------------------\n");
    printf("Time taken create_SNN_graph1 = %lf seconds\n", timeused);
    printf("---------------------------------------------------------\n");

    //printmat_int(SNN_matrix, N, N)
    //deallocation
    for (int i = 0; i < N; i++) free(table[i]);
    free(table);

    for (int i = 0; i < N; i++) free(SNN_matrix[i]);
    free(SNN_matrix);

    printf("\n");

}
#else
{
  clock_t start, end;

  int N; //total number of nodes
  char *filename1 = argv[1];
  char **table;
  int **SNN_matrix;

  printf("\n");
  printf("Timings for connectivity graph in 2D table format\n");
  printf("(without parallellization)\n");
  printf("\n");

  start = clock();
  read_graph_from_file1(filename1, &N, &table);
  end = clock();
  timeused = (double) (end-start)/CLOCKS_PER_SEC;
  printf("Time taken for read_graph_from_file1 = %lf seconds\n", timeused);
  printf("----------------------------------------------------------\n");

  //printmat_char(table, N, N);
  start = clock();
  create_SNN_graph1(N, table, &SNN_matrix);
  end = clock();
  timeused = (double) (end-start)/CLOCKS_PER_SEC;
  printf("----------------------------------------------------------\n");
  printf("Time taken create_SNN_graph1 = %lf seconds\n", timeused);
  printf("----------------------------------------------------------\n");
  //printmat_int(SNN_matrix, N, N);

  //deallocation
  for (int i = 0; i < N; i++) free(table[i]);
  free(table);

  for (int i = 0; i < N; i++) free(SNN_matrix[i]);
  free(SNN_matrix);

  printf("\n");

}
#endif

  return 0;
}
