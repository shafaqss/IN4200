#include <stdio.h>  // printf
#include <stdlib.h> //malloc and calloc
#include <stdbool.h>

//real
/*
Function: read_graph_from_file1
--------------------------------
This function reads the connecitivty graph data from the input file and stores
the graph in the form of a 2D table(matrix).

 char *filename: pointer to filename containing connectivity graph data
 int *N: number of nodes in connecitivty graph
 char ***table2D: 2D table of size NxN to store graph
*/
void read_graph_from_file1(char *filename, int *N, char ***table2D){
  int total_edges;

  FILE *datafile;
  datafile = fopen(filename, "r");
  if (datafile == NULL) {
    printf("The file %s failed to open! \n", filename);
    exit(1);
   }

  fscanf(datafile, "%*[^\n]\n"); // skip line
  fscanf(datafile, "%*[^\n]\n"); // skip line
  fscanf(datafile, "%*s %*s %d %*s %d \n", N, &total_edges);
  fscanf(datafile, "%*[^\n]\n"); // skip line
  printf("Connectivity graph information\n");
  printf("---------------------------------\n");
  printf("Total no. of nodes N, are: %d \n",*N);
  printf("Total no. of edges E, are: %d \n", 2*total_edges);
  printf("(Edges read from file are %d) \n", total_edges);
  printf("---------------------------------\n");
  printf("\n");

  //allocate table2D of size NxN
  *table2D = (char**)calloc((*N), sizeof(char**));
  for (int i=0; i<(*N) ; i++){
    (*table2D)[i] = (char*)calloc((*N), sizeof(char*));
  }

  int FromNodeId, ToNodeId;
  while (fscanf(datafile, "%d %d\n", &FromNodeId, &ToNodeId) != EOF) {
      if(ToNodeId != FromNodeId) { //make sure only the legal values are stored from file
        if( (ToNodeId < *N) && (FromNodeId < *N) && (ToNodeId > -1) && (FromNodeId > -1)) {
          (*table2D)[FromNodeId][ToNodeId] =
          (*table2D)[ToNodeId][FromNodeId] = (char)1;
        }
      }
  }
  fclose(datafile);

}//end of function
