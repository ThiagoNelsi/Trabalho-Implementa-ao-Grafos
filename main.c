#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef struct {
    int number_of_vertices;
    int number_of_edges;
    char type; // G or D (dígrafo)
    int is_weighted;
    int edges[MAX_VERTICES][MAX_VERTICES];
} Graph;

typedef struct ListItem {
    int vertex;
    int weight;
    struct ListItem * next;
} ListItem;

Graph * new_graph() {
    Graph * graph = malloc(sizeof(Graph));
    graph->number_of_vertices = 0;
    graph->number_of_edges = 0;
    graph->type = 'G';
    graph->is_weighted = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph->edges[i][j] = 0;
        }
    }
    return graph;
}

void print_graph(Graph graph) {
    printf("Number of vertices: %d\n", graph.number_of_vertices);
    printf("Number of edges: %d\n", graph.number_of_edges);
    printf("Type: %c\n", graph.type);
    printf("Is weighted: %d\n", graph.is_weighted);
    for (int i = 0; i < graph.number_of_vertices; i++) {
        for (int j = 0; j < graph.number_of_vertices; j++) {
            printf("%d ", graph.edges[i][j]);
        }
        printf("\n");
    }
}

void print_list(ListItem list[]) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (list[i].vertex == NULL) continue;
        printf("%d: ", i);
        ListItem * aux = &list[i];
        while (aux != NULL) {
            printf("%d ", aux->vertex);
            aux = aux->next;
        }
        printf("\n");
    }
}

int graph_to_txt(Graph * graph, char* filename) {
    FILE * fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    fprintf(fp, "%d %d %c %d\n", graph->number_of_vertices, graph->number_of_edges, graph->type, graph->is_weighted);
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
        if (is_weighted) {
            fscanf(fp, "%d", &weight);
            graph->edges[vi][vj] = weight;
            if (type == 'G') {
                graph->edges[vj][vi] = weight;
            }
        } else {
            graph->edges[vi][vj]++;
            if (type == 'G' && vi != vj) {
                graph->edges[vj][vi]++;
            }
        }
    }
    print_graph(*graph);
    return graph;
}

ListItem * matrix_to_list(Graph * graph) {
    ListItem * list = malloc(sizeof(ListItem) * MAX_VERTICES);

    for (int i = 0; i < graph->number_of_vertices; i++) {
        for (int j = 0; j < graph->number_of_vertices; j++) {
            if (graph->edges[i][j] == 0) continue;

            ListItem * listItem = malloc(sizeof(ListItem));
            listItem->vertex = j;
            listItem->weight = graph->edges[i][j];
            listItem->next = NULL;

            if (list[i].vertex == NULL) {
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

int * get_vertices_degrees(Graph * graph) {
    int * vertices_degrees = malloc(sizeof(int) * graph->number_of_vertices);
    for (int i = 0; i < graph->number_of_vertices; i++) {
        vertices_degrees[i] = 0;
    }
    for (int i = 0; i < graph->number_of_vertices; i++) {
        for (int j = 0; j < graph->number_of_vertices; j++) {
            if (graph->edges[i][j] != 0) {
                vertices_degrees[i]++;
            }
        }
    }
    return vertices_degrees;
}

int main() {
    char* input_filename = "input.txt";
    char* output_filename = "output.txt";
    Graph * graph = txt_to_graph(input_filename);
    graph_to_txt(graph, output_filename);

    printf("\n");

    ListItem * list = matrix_to_list(graph);
    print_list(list);

    printf("\n");

    int * vertices_degrees = get_vertices_degrees(graph);
    for (int i = 0; i < graph->number_of_vertices; i++) {
        printf("%d ", vertices_degrees[i]);
    }
    printf("\n");

    return 0;
}