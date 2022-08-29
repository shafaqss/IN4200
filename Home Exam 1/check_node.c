#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Function: DFS
--------------------------------
This function performs a depth first search(DFS) starting at the node.

 int N: number of nodes
 int *visited: array to indicate if a node has been visited(1), or not(0)
 int node: DFS will start on this node
 int main_node: this is here to prevent printing the node we are finding clusters for
 int tau: threshold value
 int *row_ptr: row pointer of connectivity graph
 int *col_idx: column indices of connectivity graph
 int *SNN_val: SNN graph in CRS format, of the given connectivity graph
*/
void DFS(int N, int *visited, int node, int main_node, int tau, int *row_ptr, int *col_idx, int *SNN_val){
  if (node != main_node) printf(" %d, ", node);
  visited[node] = 1; //mark node as visited

  for (int i=row_ptr[node] ; i < row_ptr[node+1] ; i++){
    if(visited[col_idx[i]]==0 && SNN_val[i] >= tau){
      DFS(N, visited, col_idx[i], main_node, tau, row_ptr, col_idx, SNN_val);
    }
  }
}//end of func

/*
Function: check_node
--------------------------------
This function checks for a given node(node_id), and a threshold value, tau,
whether it can be in a cluster based on the input SNN graph in CRS format.

 int node_id: node you want to check if it is in a cluster, or not
 int tau: threshold value, clusters are found for SNN >= tau
 int N: number of nodes in connectivity graph
 int *row_ptr: row pointer of connectivity graph
 int *col_idx: column indices of connectivity graph
 int *SNN_val: SNN graph in CRS format, of the given connectivity graph
*/
void check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val){
  if (node_id < 0 || node_id >= N){ // checking if the node_id is valid
    printf("ERROR:The number of nodes is in the range [0,%d)\n",N );
  }else{

    int *bool_arr = (int*)calloc(N, sizeof(int*)); //marking all nodes as unvisited
    //int main_node = node_id;
    printf("\n");
    printf("Here are the other nodes in a cluster with node:%d, and tau:%d\n", node_id,tau);
    printf("{");
    //perform a depth first search to find the clusters
    DFS(N, bool_arr, node_id, node_id, tau, row_ptr, col_idx, SNN_val);
    printf("}");
    printf("\n");

    free(bool_arr);
  }
}//end of func
