main_2D: main_2Dtable.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c
	gcc main_2Dtable.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c
	./a.out facebook.txt

main_CRS: main_CRS.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c check_node.c
	gcc main_CRS.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c check_node.c
	./a.out facebook.txt

main_2Domp: main_2Dtable.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c
	gcc -fopenmp main_2Dtable.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c
	./a.out facebook.txt

main_CRSomp: main_CRS.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c check_node.c
	gcc -fopenmp main_CRS.c read_graph_from_file1.c read_graph_from_file2.c create_SNN_graph1.c create_SNN_graph2.c check_node.c
	./a.out facebook.txt
