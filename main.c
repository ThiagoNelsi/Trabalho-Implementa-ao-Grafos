#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

typedef struct {
    int vertex;
    int weight;
} Vertex;

typedef struct EdgeQueue {
    struct Edge edge;
    struct EdgeQueue * next;
} EdgeQueue;

typedef struct {
    int * vertices_degrees;
    int * vertices_input_degrees;
    int * vertices_output_degrees;
} GraphDegree;

typedef struct Path {
    int * distance;
    int * parent;
} Path;

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

        if (graph->type == 'G') {
            ListItem * listItem = malloc(sizeof(ListItem));
            listItem->vertex = graph->edges[i].vi;
            listItem->weight = graph->edges[i].weight;
            listItem->next = NULL;

            if (list[graph->edges[i].vj].vertex == -1) {
                list[graph->edges[i].vj] = *listItem;
            } else {
                ListItem * aux = &list[graph->edges[i].vj];
                while (aux->next != NULL) {
                    aux = aux->next;
                }
                aux->next = listItem;
            }
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

GraphDegree * calculate_vertices_degrees(Graph * graph) {
    GraphDegree * graphDegree = malloc(sizeof(GraphDegree));
    graphDegree->vertices_degrees = malloc(sizeof(int) * graph->number_of_vertices);
    graphDegree->vertices_input_degrees = malloc(sizeof(int) * graph->number_of_vertices);
    graphDegree->vertices_output_degrees = malloc(sizeof(int) * graph->number_of_vertices);

    for (int i = 0; i < graph->number_of_vertices; i++) {
        graphDegree->vertices_degrees[i] = 0;
        graphDegree->vertices_input_degrees[i] = 0;
        graphDegree->vertices_output_degrees[i] = 0;
    }

    for (int i = 0; i < graph->number_of_edges; i++) {
        graphDegree->vertices_degrees[graph->edges[i].vi]++;
        graphDegree->vertices_degrees[graph->edges[i].vj]++;
        graphDegree->vertices_input_degrees[graph->edges[i].vj]++;
        graphDegree->vertices_output_degrees[graph->edges[i].vi]++;
    }

    return graphDegree;
}

int queue_includes(EdgeQueue * queue, Edge edge) {
    EdgeQueue * aux = queue;

    if (aux == NULL) return 0;

    while (aux != NULL) {
        if (aux->edge.vi == edge.vi && aux->edge.vj == edge.vj) {
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}

void add_edge_to_queue(ListItem * adjacency_list, EdgeQueue ** queue, int * visited, int vertex) {
    ListItem * aux = &adjacency_list[vertex];

    while(aux != NULL) {
        Edge edge;
        edge.vi = vertex;
        edge.vj = aux->vertex;

        Edge edge2;
        edge2.vi = aux->vertex;
        edge2.vj = vertex;

        if (visited[aux->vertex] == 0 && !queue_includes(*queue, edge) && !queue_includes(*queue, edge2)) {
            EdgeQueue * edgeQueue = malloc(sizeof(EdgeQueue));
            edgeQueue->edge.vi = vertex;
            edgeQueue->edge.vj = aux->vertex;
            edgeQueue->edge.weight = aux->weight;
            edgeQueue->next = NULL;

            if (*queue == NULL) {
                *queue = edgeQueue;
            } else {
                EdgeQueue * aux_queue = (*queue);
                while (aux_queue->next != NULL) {
                    aux_queue = aux_queue->next;
                }
                aux_queue->next = edgeQueue;
            }
        }
        aux = aux->next;
    }
}

Edge remove_min_vertex(EdgeQueue ** queue, int * visited) {
    // get min vertex and remove from queue
    Edge min_edge;
    min_edge.vi = -1;
    min_edge.weight = INT_MAX;

    EdgeQueue * aux = (*queue);
    EdgeQueue * prev = NULL;
    EdgeQueue * prev_min_edge = NULL;
    while (aux != NULL) {
        if (aux->edge.weight < min_edge.weight && visited[aux->edge.vj] == 0) {
            min_edge.vi = aux->edge.vi;
            min_edge.vj = aux->edge.vj;
            min_edge.weight = aux->edge.weight;
            prev_min_edge = prev;
        }
        prev = aux;
        aux = aux->next;
    }

    if (prev_min_edge == NULL) {
        (*queue) = (*queue)->next;
    } else {
        prev_min_edge->next = prev_min_edge->next->next;
    }

    visited[min_edge.vj] = 1;

    return min_edge;
}

void print_edge_queue(EdgeQueue * queue) {
    EdgeQueue * aux = queue;
    while (aux != NULL) {
        printf("(%d|%d|%d) ", aux->edge.vi, aux->edge.vj, aux->edge.weight);
        aux = aux->next;
    }
    printf("\n");
}

int all_edges_visited(int * visited, int size) {
    for (int i = 0; i < size; i++) {
        if (visited[i] == 0) {
            return 0;
        }
    }
    return 1;
}

int * generate_minimun_spanning_tree(Graph * graph, int start_vertex) {
    ListItem * adjacency_list = edges_to_list(graph);

    // Inicialization
    int * key = malloc(sizeof(int) * graph->number_of_vertices);
    int * parent = malloc(sizeof(int) * graph->number_of_vertices);
    int * visited = malloc(sizeof(int) * graph->number_of_vertices);

    for (int i = 0; i < graph->number_of_vertices; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
        visited[i] = 0;
    }

    key[start_vertex] = 0;
    visited[start_vertex] = 1;

    EdgeQueue * queue = malloc(sizeof(EdgeQueue));
    queue = NULL;

    add_edge_to_queue(adjacency_list, &queue, visited, start_vertex);

    while (queue != NULL && !all_edges_visited(visited, graph->number_of_vertices)) {
        Edge min_vertex = remove_min_vertex(&queue, visited);
        key[min_vertex.vj] = min_vertex.weight;
        parent[min_vertex.vj] = min_vertex.vi;

        add_edge_to_queue(adjacency_list, &queue, visited, min_vertex.vj);
    }

    return parent;
}

Path * find_shortest_path(Graph * graph, int start_vertex) {
    Path * path = malloc(sizeof(Path));

    int * distance = malloc(sizeof(int) * graph->number_of_vertices);
    int * visited = malloc(sizeof(int) * graph->number_of_vertices);
    int * parent = malloc(sizeof(int) * graph->number_of_vertices);

    for (int i = 0; i < graph->number_of_vertices; i++) {
        distance[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    distance[start_vertex] = 0;

    // Dijkstra
    for (int count = 0; count < graph->number_of_vertices - 1; count++) {
        int min_distance = INT_MAX;
        int min_index = -1;

        for (int v = 0; v < graph->number_of_vertices; v++) {
            if (!visited[v] && distance[v] < min_distance) {
                min_distance = distance[v];
                min_index = v;
            }
        }

        visited[min_index] = 1;

        for (int v = 0; v < graph->number_of_vertices; v++) {
            if (!visited[v] && graph->adjacency_matrix[min_index][v] != 0 &&
                distance[min_index] + graph->adjacency_matrix[min_index][v] < distance[v]) {
                distance[v] = distance[min_index] + graph->adjacency_matrix[min_index][v];
                parent[v] = min_index;
            }
        }
    }

    path->distance = distance;
    path->parent = parent;

    return path;
}

void print_shortest_paths(Path * path, int start_vertex, int number_of_vertices) {
    printf("%-10s %-10s %-10s\n", "Vertex", "Distance", "Path");

    for (int i = 0; i < number_of_vertices; i++) {
        if (i == start_vertex) {
            printf("%-10d %-10d %-10s\n", i, 0, "-");
        } else if (path->distance[i] == INT_MAX) {
            printf("%-10d %-10s %-10s\n", i, "INF", "-");
        } else {
            printf("%-10d %-10d %d", i, path->distance[i], i);

            int parent = path->parent[i];
            while (parent != start_vertex) {
                printf(" <- %d", parent);
                parent = path->parent[parent];
            }

            printf(" <- %d\n", start_vertex);
        }
    }
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
    GraphDegree * vertices_degrees = calculate_vertices_degrees(graph);
    if (graph->type == 'G') {
        printf("%-10s %-10s\n", "Vertex", "Degree");
        for (int i = 0; i < graph->number_of_vertices; i++) {
            printf("%-10d %-10d\n", i, vertices_degrees->vertices_degrees[i]);
        }
    } else {
        printf("%-10s %-10s %-10s\n", "Vertex", "Input", "Output");
        for (int i = 0; i < graph->number_of_vertices; i++) {
            printf("%-10d %-10d %-10d\n", i, vertices_degrees->vertices_input_degrees[i], vertices_degrees->vertices_output_degrees[i]);
        }
    }
    printf("\n");

    printf("[6/6] Generating Minimum Spanning Tree...\n");
    int * tree = generate_minimun_spanning_tree(graph, 0);
    printf("%-10s %-10s\n", "Vertex", "Parent");
    for (int i = 0; i < graph->number_of_vertices; i++) {
        printf("%-10d %-10d\n", i, tree[i]);
    }
    // printf sum
    int sum = 0;
    for (int i = 0; i < graph->number_of_vertices; i++) {
        sum += graph->adjacency_matrix[i][tree[i]];
    }
    printf("Sum: %d\n", sum);


    printf("\n");

    printf("[7/7] Finding shortest path...\n");
    Path * path = find_shortest_path(graph, 0);
    print_shortest_paths(path, 0, graph->number_of_vertices);

    return 0;
}