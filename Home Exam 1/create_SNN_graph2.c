#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Function: create_SNN_graph2
--------------------------------
This function creates a shared nearest neighbours(SNN) graph when given the
connectivity graph in CRS fromat.

 int N: number of nodes in connecitivty graph
 int *row_ptr: row pointer of connectivity graph in CRS format
 int *col_idx: column idices of connectivity graph in CRS format
 int **SNN_val: SNN graph in CRS format, of the given connecitivty graph
*/
void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val){
  int col_idx_length = row_ptr[N];
  printf("The length of col_idx is: %d\n", col_idx_length);
  printf("The length of row_ptr is: %d\n", N);
  printf("The length of SNN_val is: %d\n", col_idx_length);

  //allocate SNN_val array
  *SNN_val = (int*)calloc(col_idx_length, sizeof(int*));

  #if defined(_OPENMP)
  {
    //#pragma omp parallel
    //{
    #pragma omp parallel for
    for (int i = 0; i < N ; i++){
      for (int j = row_ptr[i]; j < row_ptr[i+1]; j++){
        //getting the node id at the col_idx[j]
        int node = col_idx[j];
        //going through the neighbours of node and i, counting the common
        //neighbours and adding them in the SNN_val at index j.
        for (int k = row_ptr[i]; k < row_ptr[i+1] ; k++){
          for(int m = row_ptr[node]; m < row_ptr[node+1] ; m++){
            if (col_idx[k] == col_idx[m]){
              (*SNN_val)[j]++;
            }
          }
        }
      }
    }//end of all for loops
//  }
}
#else
{
  //if parallelization is not used
  for (int i = 0; i < N ; i++){
    for (int j = row_ptr[i]; j < row_ptr[i+1]; j++){
      int node = col_idx[j];
      for (int k = row_ptr[i]; k < row_ptr[i+1] ; k++){
        for(int m = row_ptr[node]; m < row_ptr[node+1] ; m++){
          if (col_idx[k] == col_idx[m]){
            (*SNN_val)[j]++;
          }
        }
      }
    }
  }//end of all for loops
}
#endif
}//end of function
