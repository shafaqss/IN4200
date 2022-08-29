#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Function: merge
-------------------
Merges to subarray of col_idx. The first subarray is from col_idx[l,...,m], and
the second subarray is from col_idx[m+1,...,r]. This decribes the input
arguments of the function.
*/
void merge(int *col_idx, int l, int m, int r){
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  //allocate temporary arrays
  int *left_arr = (int*)calloc(n1, sizeof(int));
  int *right_arr = (int*)calloc(n2, sizeof(int));

  for(i=0; i<n1; i++){
    left_arr[i] = col_idx[l+i];
  }
  for (j=0; j<n2; j++){
    right_arr[j] = col_idx[m+1+j];
  }

  i=0; j=0; k=l;
  while (i < n1 && j < n2) {
      if (left_arr[i] <= right_arr[j]) {
          col_idx[k] = left_arr[i];
          i++;
      }
      else {
          col_idx[k] = right_arr[j];
          j++;
      }
      k++;
  }

  //copy the remaining elements of left_arr and right_arr (if there are any)
  while (i < n1) {
      col_idx[k] = left_arr[i];
      i++;
      k++;
  }
  while (j < n2) {
      col_idx[k] = right_arr[j];
      j++;
      k++;
  }
  free(left_arr);
  free(right_arr);

}//end of func

/*
Function: mergesort
-------------------
Sorting algorithm to sort given array in increasing order,i.e from smallest
to largest value. Uses function merge.

 int *col_idx: input array to be sorted
 int l: left index of the sub-array of col_idx to be sorted
 int r: right index of the sub-array of col_idx to be sorted
*/
void mergesort(int *col_idx, int l, int r){
  //printf("calllll\n");
  if (l < r){
    int m = l + (r-l) / 2;

    mergesort(col_idx, l , m);
    mergesort(col_idx, m+1, r);
    merge(col_idx, l, m, r);
  }
} //end of function

/*
Function: read_graph_from_file2
-------------------------------
This function reads the connectivity graph data from the input file, and stores
the graph in the form of compressed row storage(CRS).

 char *filename: pointer to filename containing connectivity graph data
 int *N: number of nodes in connecitivty graph
 int **row_ptr: row pointer of length N+1
 int **col_idx: array that stores column indices of elements of length 2*(N_edges)
*/
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx) {
  int edges;

  FILE *datafile;
  datafile = fopen(filename, "r");
  if (datafile == NULL) {
    printf("The file %s failed to open! \n", filename);
    exit(1);
   }

   fscanf(datafile, "%*[^\n]\n"); // skip line
   fscanf(datafile, "%*[^\n]\n");
   fscanf(datafile, "%*s %*s %d %*s %d \n", N, &edges);
   fscanf(datafile, "%*[^\n]\n");
   printf("Connectivity graph information\n");
   printf("---------------------------------\n");
   printf("Total no. of nodes N, are: %d \n",*N);
   printf("Total no. of edges E, are: %d \n", 2*edges);
   printf("(Edges read from are file %d) \n", edges);
   printf("---------------------------------\n");
   printf("\n");

  //allocate the arrays
  *col_idx = malloc((edges*2) * sizeof(int));
  *row_ptr = malloc((*N + 1) * sizeof(int));
  (*row_ptr)[0] = 0; //the first element in row_ptr is always 0.

  //creating extra arrays to store the nodes from index and to index
  int *to_nodes = malloc((edges*2) * sizeof(int));
  int *from_nodes = malloc((edges*2) * sizeof(int));

  int FromNodeId, ToNodeId;
  int d = 0;
  while (fscanf(datafile, "%d %d\n", &FromNodeId, &ToNodeId) != EOF) {
    //check if the node id's are legal values, and then store them
      if(FromNodeId != ToNodeId) {
        if((ToNodeId < *N) && (FromNodeId < *N) && (FromNodeId > -1) && (ToNodeId > -1)){
          (*row_ptr)[ToNodeId + 1]++;  //undirected graph
          to_nodes[d]     = ToNodeId;
          from_nodes[d]   = FromNodeId;
          d++;
          (*row_ptr)[FromNodeId + 1]++;
          to_nodes[d]     = FromNodeId;
          from_nodes[d]   = ToNodeId;
          d++;
        }
      }
  }
  fclose(datafile);

  //going through the to and from arrays to make the col_idx
  int col_counter = 0;
  for (int i = 0; i < (*N); i++) {
      for (int j = 0; j < (edges*2); j++){
          if (to_nodes[j] == i) {
              (*col_idx)[col_counter] = from_nodes[j];
              col_counter++;
          }
      }
  }
  free(to_nodes);
  free(from_nodes);

  //Making the values of the row_ptr correct
  for (int k = 1; k < (*N+1); k++) {
      (*row_ptr)[k] += (*row_ptr)[k - 1];
  }

  //sorting the col_idx by using merge sort
  for (int i=0 ; i<*N ; i++){
    mergesort((*col_idx), (*row_ptr)[i], (*row_ptr)[i+1]-1 );
  }

}//end of function
