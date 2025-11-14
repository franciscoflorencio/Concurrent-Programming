#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define MAX_THREADS 16

// Struct to represent a weighted edge in the graph
typedef struct {
    int src, dest, weight;
} Edge;

// Struct to hold graph information
typedef struct {
    int V, E;
    Edge* edges;
} Graph;

// Struct to pass arguments to each thread
typedef struct {
    int thread_id;
    int start_edge;
    int num_edges_for_thread;
    int* distances;
    Graph* graph;
    pthread_barrier_t* barrier;
} ThreadData;


/**
 * @brief The core function executed by each thread.
 *
 * This function performs the relaxation step on a subset of the graph's edges
 * for V-1 iterations. It synchronizes with other threads using a barrier
 * after each iteration.
 */
void* bellman_ford_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    Graph* graph = data->graph;
    int V = graph->V;
    int start_edge = data->start_edge;
    int end_edge = start_edge + data->num_edges_for_thread;

    // Main Bellman-Ford loop: V-1 iterations
    for (int i = 0; i < V - 1; ++i) {
        // Relax edges assigned to this thread
        for (int j = start_edge; j < end_edge; ++j) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;

            if (data->distances[u] != INT_MAX && data->distances[u] + weight < data->distances[v]) {
                data->distances[v] = data->distances[u] + weight;
            }
        }
        // Synchronize with other threads before starting the next iteration
        pthread_barrier_wait(data->barrier);
    }

    pthread_exit(NULL);
}


/**
 * @brief Main function to run the parallel Bellman-Ford algorithm.
 *
 * @param graph The graph object.
 * @param src The source vertex.
 * @param num_threads The number of threads to use.
 */
void parallel_bellman_ford(Graph* graph, int src, int num_threads) {
    int V = graph->V;
    int E = graph->E;
    int* distances = (int*)malloc(V * sizeof(int));

    // Step 1: Initialize distances
    for (int i = 0; i < V; ++i) {
        distances[i] = INT_MAX;
    }
    distances[src] = 0;

    // Thread management variables
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    pthread_barrier_t barrier;

    // Initialize the barrier to synchronize num_threads
    pthread_barrier_init(&barrier, NULL, num_threads);

    // Step 2: Create threads and distribute the work
    int edges_per_thread = E / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].start_edge = i * edges_per_thread;
        // The last thread takes any remaining edges
        thread_data[i].num_edges_for_thread = (i == num_threads - 1) ? (E - i * edges_per_thread) : edges_per_thread;
        thread_data[i].distances = distances;
        thread_data[i].graph = graph;
        thread_data[i].barrier = &barrier;

        pthread_create(&threads[i], NULL, bellman_ford_thread, (void*)&thread_data[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the barrier
    pthread_barrier_destroy(&barrier);

    // Step 3: Check for negative-weight cycles
    for (int i = 0; i < E; ++i) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
            printf("Graph contains a negative-weight cycle!\n");
            free(distances);
            return;
        }
    }

    // Print the result
    printf("Vertex distances from source %d:\n", src);
    for (int i = 0; i < V; ++i) {
        if (distances[i] == INT_MAX) {
            printf("%d -> %d: INF\n", src, i);
        } else {
            printf("%d -> %d: %d\n", src, i, distances[i]);
        }
    }
    
    free(distances);
}


int main() {
    int V = 5;
    int E = 8;
    int NUM_THREADS = 2; // Number of threads to use

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edges = (Edge*)malloc(E * sizeof(Edge));

    // Example graph edges
    // src, dest, weight
    graph->edges[0] = (Edge){0, 1, -1};
    graph->edges[1] = (Edge){0, 2, 4};
    graph->edges[2] = (Edge){1, 2, 3};
    graph->edges[3] = (Edge){1, 3, 2};
    graph->edges[4] = (Edge){1, 4, 2};
    graph->edges[5] = (Edge){3, 2, 5};
    graph->edges[6] = (Edge){3, 1, 1};
    graph->edges[7] = (Edge){4, 3, -3};
    graph->edges[8] = (Edge){-1, -1, 0}; 

    int source_vertex = 0;
    
    if (NUM_THREADS > E) {
        printf("Warning: Number of threads > number of edges. Setting threads = edges.\n");
        NUM_THREADS = E;
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS);

    free(graph->edges);
    free(graph);

    return 0;
}