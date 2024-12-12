#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <climits>

struct Edge {
    int to;
    int capacity;
    Edge* reverse;
};

struct Graph {
    int vertex_count;
    std::vector<Edge*>* adj_list;
};

Graph* create_graph(int vertex_count) {
    Graph* graph = new Graph;
    graph->vertex_count = vertex_count;
    graph->adj_list = new std::vector<Edge*>[vertex_count];
    return graph;
}

void add_edge(Graph* graph, int from, int to, int capacity) {
    Edge* forward = new Edge{to, capacity, nullptr};
    Edge* backward = new Edge{from, 0, forward};
    forward->reverse = backward;
    graph->adj_list[from].push_back(forward);
    graph->adj_list[to].push_back(backward);
}

bool dfs(Graph* graph, int source, int sink, Edge** path, bool* visited) {
    if (source == sink) return true;

    visited[source] = true;
    for (Edge* edge : graph->adj_list[source]) {
        if (!visited[edge->to] && edge->capacity > 0) {
            path[edge->to] = edge;
            if (dfs(graph, edge->to, sink, path, visited)) return true;
        }
    }
    return false;
}

int ford_fulkerson(Graph* graph, int source, int sink) {
    int max_flow = 0;
    Edge** path = new Edge*[graph->vertex_count];
    bool* visited = new bool[graph->vertex_count];

    while (true) {
        std::fill(path, path + graph->vertex_count, nullptr);
        std::fill(visited, visited + graph->vertex_count, false);

        if (!dfs(graph, source, sink, path, visited)) break;

        int flow = INT_MAX;
        for (int v = sink; v != source; v = path[v]->reverse->to) {
            flow = std::min(flow, path[v]->capacity);
        }

        for (int v = sink; v != source; v = path[v]->reverse->to) {
            path[v]->capacity -= flow;
            path[v]->reverse->capacity += flow;
        }

        max_flow += flow;
    }

    delete[] path;
    delete[] visited;
    return max_flow;
}

void free_graph(Graph* graph) {
    for (int i = 0; i < graph->vertex_count; ++i) {
        for (Edge* edge : graph->adj_list[i]) {
            delete edge;
        }
    }
    delete[] graph->adj_list;
    delete graph;
}

int main(int argc, char* argv[]) {
    // проверка входных параметров
    if (argc < 2) {
        std::cerr << "Программе не передано имя входного файла!\n";
        return 1;
    }

    if (argc == 3 || argc > 4) {
        std::cerr << "Ошибка входных параметров!\n";
        return 1;
    }

    const char* FILE2;
    if (argc == 4) {
        FILE2 = argv[3];
    }
    else {
        FILE2 = "output.txt";
    }

    // работа с входным файлом
    const char* FILE1 = argv[1];
    std::ifstream fin(FILE1);

    if (!fin) {
        std::cerr << "Ошибка открытия файла!\n";
        return 1;
    }

    int16_t vertex_count;
    fin.read(reinterpret_cast<char*>(&vertex_count), sizeof(vertex_count));

    Graph* graph = create_graph(vertex_count);

    int16_t from, to, capacity;
    while (fin.read(reinterpret_cast<char*>(&from), sizeof(from)) &&
           fin.read(reinterpret_cast<char*>(&to), sizeof(to)) &&
           fin.read(reinterpret_cast<char*>(&capacity), sizeof(capacity))) {
        add_edge(graph, from, to, capacity);
    }

    int source = 0; // Исток
    int sink = vertex_count - 1; // Сток
    std::cout << "Максимальный поток: " << ford_fulkerson(graph, source, sink) << std::endl;

    free_graph(graph);
    return 0;
}
