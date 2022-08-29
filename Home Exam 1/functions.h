#ifndef FUNCTIONS_H
#define FUNCTIONS_H
void read_graph_from_file1(char *filename, int *N, char ***table2D);
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx);
void create_SNN_graph1(int N, char **table2D, int ***SNN_table);
void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val);
void check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val);

//print matrix of type int
void printmat_int(int **A, int n, int m){
    for (size_t i = 0; i < n; ++i){
        printf("| ");
        for (size_t j = 0; j < m; ++j){
            printf("%d ", A[i][j]);
        }
        printf("|\n");
    }
}

//print matrix of type char
void printmat_char(char **A, int n, int m){
    for (int i = 0; i < n; ++i){
        printf("| ");
        for (int j = 0; j < m; ++j){
            printf("%d ", A[i][j]);
        }
        printf("|\n");
    }
}

//print 1D array of type int
void printvec(int *a, int n){
    printf("[%d,", a[0]);
    for (size_t i = 1; i < n-1; i++) {
        printf(" %d,", a[i]);
    }
    printf(" %d]\n", a[n-1]);
}

#endif
