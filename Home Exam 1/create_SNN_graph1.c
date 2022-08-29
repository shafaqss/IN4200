#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*
Function: create_SNN_graph1
--------------------------------
This function creates a shared nearest neighbours(SNN) graph when given the
connectivity graph in the form of a 2D table.

 int N: number of nodes in connecitivty graph
 char **table2D: connecitivity graph stored as a 2D table of size NxN
 int ***SNN_table: SNN graph in 2D table form of the given connecitivty graph
*/
void create_SNN_graph1(int N, char **table2D, int ***SNN_table){
  //allocating the 2D SNN table (same size as the 2D table)
  *SNN_table = (int**)calloc(N, sizeof(int**));
  for (int i=0; i<(N) ; i++){
    (*SNN_table)[i] = (int*)calloc(N, sizeof(int*));
  }
  printf("SNN 2D table allocated of size: %d x %d\n", N, N);
  #if defined(_OPENMP)
  {
      //printf("In parallel region\n");
    //The 2Dtable(matrix) is symmetric,thus we only need to iterate the upper
    //triangular part of the matirx, in addition the diagonal entries are always 0.
    #pragma omp parallel for
    for (int i=0; i<N-1 ; i++){
      for (int j=i+1; j<N ; j++){
        //only count SNN if node i and j are connected, we count how many neighbours
        // node i and j have in common.
        if (table2D[i][j]==1){
          for (int k=0; k<N ; k++){
            if (i!=k && j!=k && table2D[i][k]==1 && table2D[j][k]==1){
              (*SNN_table)[i][j] ++;
              (*SNN_table)[j][i] ++;
            }
          }
        }
      }
    }//end of for loops

  }
  #else
  {
    //if parallelization is not used
    for (int i=0; i<N-1 ; i++){
      for (int j=i+1; j<N ; j++){
        if (table2D[i][j]==1){
          for (int k=0; k<N ; k++){
            if (i!=k && j!=k && table2D[i][k]==1 && table2D[j][k]==1){
              (*SNN_table)[i][j] ++;
              (*SNN_table)[j][i] ++;
            }
          }
        }
      }
    }//end of for loops

  }
  #endif

}//end of function
