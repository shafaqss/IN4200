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
    int *row_ptr;
    int *col_idx;
    int *SNN_array;

    printf("\n");
    printf("Timings for connectivity graph in CRS format\n");
    printf("(with parallellization)\n");
    printf("\n");

    start = omp_get_wtime();
    read_graph_from_file2(filename1, &N, &row_ptr, &col_idx);
    end = omp_get_wtime();
    timeused = end-start;
    printf("Time taken for read_graph_from_file2 = %lf seconds\n", timeused);
    printf("----------------------------------------------------------\n");

    //omp_set_num_threads(20);   //OpenMP threads
    start = omp_get_wtime();
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_array);
    end = omp_get_wtime();
    timeused = end-start;
    printf("----------------------------------------------------------\n");
    printf("Time taken for create_SNN_graph2 = %lf seconds\n", timeused);
    printf("----------------------------------------------------------\n");

    int len_col = row_ptr[N];
    //printf("The col_idx:\n");
    //printvec(col_idx, len_col);
    //printf("The row_ptr:\n");
    //printvec(row_ptr, N+1);
    //printf("The SNN_array:\n");
    //printvec(SNN_array, len_col);

    start = omp_get_wtime();
    //check_node(node , tau)
    check_node(600, 100, N, row_ptr, col_idx, SNN_array);
    end = omp_get_wtime();
    timeused = end-start;

    printf("\n");
    printf("----------------------------------------------------------\n");
    printf("Time taken for check_node = %lf seconds\n", timeused);
    printf("----------------------------------------------------------\n");
    printf("\n");

    free(row_ptr);
    free(col_idx);
    free(SNN_array);

  }
  #else
  {
    clock_t start, end;

    int N; //total number of nodes
    char *filename1 = argv[1];
    int *row_ptr;
    int *col_idx;
    int *SNN_array;

    printf("\n");
    printf("Timings for connectivity graph in CRS format\n");
    printf("(without parallellization)\n");
    printf("\n");

    start = clock();
    read_graph_from_file2(filename1, &N, &row_ptr, &col_idx);
    end = clock();
    timeused = (double) (end-start)/CLOCKS_PER_SEC;
    printf("Time taken for read_graph_from_file2 = %lf seconds\n", timeused);
    printf("----------------------------------------------------------\n");

    start = clock();
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_array);
    end = clock();
    timeused = (double) (end-start)/CLOCKS_PER_SEC;
    printf("----------------------------------------------------------\n");
    printf("Time taken for create_SNN_graph2 = %lf seconds\n", timeused);
    printf("----------------------------------------------------------\n");

    int len_col = row_ptr[N];
    //printf("The col_idx:\n");
    //printvec(col_idx, len_col);
    //printf("The row_ptr:\n");
    //printvec(row_ptr, N+1);
    //printf("The SNN_array:\n");
    //printvec(SNN_array, len_col);

    start = clock();
    //check_node(node , tau)
    check_node(600, 100, N, row_ptr, col_idx, SNN_array);
    end = clock();
    timeused = (double) (end-start)/CLOCKS_PER_SEC;

    printf("\n");
    printf("----------------------------------------------------------\n");
    printf("Time taken for check_node = %lf seconds\n", timeused);
    printf("----------------------------------------------------------\n");
    printf("\n");

    free(row_ptr);
    free(col_idx);
    free(SNN_array);

  }
  #endif

  return 0;
}
