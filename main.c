#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef struct ListItem {
    int vertex;
    int weight;
    struct ListItem * next;
} ListItem;

typedef struct Edge {
    int vi;
    int vj;
    int weight;
} Edge;

typedef struct {
    int number_of_vertices;
    int number_of_edges;
    char type; // G or D (dígrafo)
    int is_weighted;
    int adjacency_matrix[MAX_VERTICES][MAX_VERTICES];
    ListItem * adjacency_list;
    Edge edges[MAX_VERTICES];
} Graph;

Graph * new_graph() {
    Graph * graph = malloc(sizeof(Graph));
    graph->number_of_vertices = 0;
    graph->number_of_edges = 0;
    graph->type = 'G';
    graph->is_weighted = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph->adjacency_matrix[i][j] = 0;
        }
    }
    return graph;
}

void print_graph(Graph graph) {
    printf("%-10s %-10s %-10s %-10s\n", "Vertices", "Edges", "Type", "Weighted");
    printf("%-10d %-10d %-10c %-10d\n", graph.number_of_vertices, graph.number_of_edges, graph.type, graph.is_weighted);

    printf("\n");

    printf("Edges:\n");
    for (int i = 0; i < graph.number_of_edges; i++) {
        printf("%d %d", graph.edges[i].vi, graph.edges[i].vj);
        if (graph.is_weighted) {
            printf(" %d", graph.edges[i].weight);
        }
        printf("\n");
    }

    printf("\n");

    printf("Adjacency matrix:\n");

    for (int i = 0; i < graph.number_of_vertices; i++) {
        for (int j = 0; j < graph.number_of_vertices; j++) {
            printf("%d ", graph.adjacency_matrix[i][j]);
        }
        printf("\n");
    }
}

void print_list(ListItem list[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d: ", i);
        ListItem * aux = &list[i];
        while (aux != NULL && aux->vertex != -1) {
            printf("(%d|%d)  ", aux->vertex, aux->weight);
            aux = aux->next;
        }
        printf("\n");
    }
}

ListItem * matrix_to_list(Graph * graph) {
    ListItem * list = malloc(sizeof(ListItem) * MAX_VERTICES);

    for (int i = 0; i < MAX_VERTICES; i++) {
        list[i].vertex = -1;
        list[i].weight = -1;
        list[i].next = NULL;
    }

    for (int i = 0; i < graph->number_of_vertices; i++) {
        for (int j = 0; j < graph->number_of_vertices; j++) {
            if (graph->adjacency_matrix[i][j] == 0) continue;

            ListItem * listItem = malloc(sizeof(ListItem));
            listItem->vertex = j;
            listItem->weight = graph->adjacency_matrix[i][j];
            listItem->next = NULL;

            if (list[i].vertex == -1) {
                list[i] = *listItem;
            } else {
                ListItem * aux = &list[i];
                while (aux->next != NULL) {
                    aux = aux->next;
                }
                aux->next = listItem;
            }
        }
    }
    return list;
}

ListItem * edges_to_list(Graph * graph) {
    ListItem * list = malloc(sizeof(ListItem) * MAX_VERTICES);

    for (int i = 0; i < MAX_VERTICES; i++) {
        list[i].vertex = -1;
        list[i].weight = -1;
        list[i].next = NULL;
    }

    for (int i = 0; i < graph->number_of_edges; i++) {
        ListItem * listItem = malloc(sizeof(ListItem));
        listItem->vertex = graph->edges[i].vj;
        listItem->weight = graph->edges[i].weight;
        listItem->next = NULL;

        if (list[graph->edges[i].vi].vertex == -1) {
            list[graph->edges[i].vi] = *listItem;
        } else {
            ListItem * aux = &list[graph->edges[i].vi];
            while (aux->next != NULL) {
                aux = aux->next;
            }
            aux->next = listItem;
        }
    }
    return list;
}

int ** list_to_matrix(ListItem * list) {
    int ** matrix = malloc(sizeof(int*) * MAX_VERTICES);

    for (int i = 0; i < MAX_VERTICES; i++) {
        matrix[i] = malloc(sizeof(int) * MAX_VERTICES);
        for (int j = 0; j < MAX_VERTICES; j++) {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < MAX_VERTICES && list != NULL; i++) {
        ListItem * node = &list[i];
        while (node != NULL) {
            matrix[i][node->vertex] = node->weight;
            node = node->next;
        }
    }
    return matrix;
}

int graph_to_txt(Graph * graph, char* filename) {
    FILE * fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    fprintf(fp, "%d %d %c %d\n", graph->number_of_vertices, graph->number_of_edges, graph->type, graph->is_weighted);
    printf("%d %d %c %d\n", graph->number_of_vertices, graph->number_of_edges, graph->type, graph->is_weighted);

    for (int i = 0; i < graph->number_of_edges; i++) {
        fprintf(fp, "%d %d", graph->edges[i].vi, graph->edges[i].vj);
        printf("%d %d", graph->edges[i].vi, graph->edges[i].vj);
        if (graph->is_weighted) {
            fprintf(fp, " %d", graph->edges[i].weight);
            printf(" %d", graph->edges[i].weight);
        }

        fprintf(fp, "\n");
        printf("\n");
    }

    return 1;
}

Graph * txt_to_graph(char* filename) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return NULL;
    }
    int number_of_vertices, number_of_edges, is_weighted;
    char type; // G or D (dígrafo)

    Graph * graph = new_graph();

    fscanf(fp, "%d %d %c %d", &number_of_vertices, &number_of_edges, &type, &is_weighted);

    graph->number_of_vertices = number_of_vertices;
    graph->number_of_edges = number_of_edges;
    graph->type = type;
    graph->is_weighted = is_weighted;

    for (int i = 0; i < number_of_edges; i++) {
        int vi, vj, weight;
        fscanf(fp, "%d %d", &vi, &vj);
        graph->edges[i].vi = vi;
        graph->edges[i].vj = vj;

        if (is_weighted) {
            fscanf(fp, "%d", &weight);
            graph->adjacency_matrix[vi][vj] = weight;
            graph->edges[i].weight = weight;

            if (type == 'G') {
                graph->adjacency_matrix[vj][vi] = weight;
            }
        } else {
            graph->adjacency_matrix[vi][vj]++;
            if (type == 'G' && vi != vj) {
                graph->adjacency_matrix[vj][vi]++;
            }
        }
    }
    print_graph(*graph);
    return graph;
}

int * calculate_vertices_degrees(Graph * graph) {
    int * vertices_degrees = malloc(sizeof(int) * graph->number_of_vertices);
    for (int i = 0; i < graph->number_of_vertices; i++) {
        vertices_degrees[i] = 0;
    }
    for (int i = 0; i < graph->number_of_edges; i++) {
        vertices_degrees[graph->edges[i].vi]++;
    }
    return vertices_degrees;
}

Graph * generate_minimun_spanning_tree(Graph * graph) {
    ListItem * adjacency_list = edges_to_list(graph);
    print_list(adjacency_list, graph->number_of_vertices);
}

int main() {
    char* input_filename = "input.txt";
    char* output_filename = "output.txt";

    printf("[1/5] Reading file...\n");
    Graph * graph = txt_to_graph(input_filename);

    printf("\n");

    printf("[2/5] Writing file...\n");
    graph_to_txt(graph, output_filename);

    printf("\n");

    printf("[3/5] Converting matrix to list...\n");
    graph->adjacency_list = matrix_to_list(graph);
    print_list(graph->adjacency_list, graph->number_of_vertices);

    printf("\n");

    printf("[4/5] Converting list to matrix...\n");
    int ** matrix = list_to_matrix(graph->adjacency_list);
    for (int i = 0; i < graph->number_of_vertices; i++) {
        for (int j = 0; j < graph->number_of_vertices; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("[5/5] Calculating vertices degrees...\n");
    int * vertices_degrees = calculate_vertices_degrees(graph);
    printf("%-10s %-10s\n", "Vertex", "Degree");
    for (int i = 0; i < graph->number_of_vertices; i++) {
        printf("%-10d %-10d\n", i, vertices_degrees[i]);
    }
    printf("\n");

    printf("[6/6] Generating Minimum Spanning Tree...\n");
    generate_minimun_spanning_tree(graph);

    return 0;
}