#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <queue>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Edge {
    int to;
    int capacity;
    int flow;
    Edge(int t, int c) : to(t), capacity(c), flow(0) {}
};

class Graph {
public:
    int V;
    vector<Edge>* adj;
    unordered_map<int, int> in_degree;  // Количество входящих рёбер для каждой вершины
    unordered_map<int, int> out_degree; // Количество исходящих рёбер для каждой вершины

    Graph(int V) {
        this->V = V;
        adj = new vector<Edge>[V];
    }

    void addEdge(int u, int v, int capacity) {
        adj[u].push_back(Edge(v, capacity));
        adj[v].push_back(Edge(u, 0));  // Обратное ребро с нулевой пропускной способностью
        out_degree[u]++;
        in_degree[v]++;
    }

    bool bfs(int s, int t, vector<int>& parent) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int i = 0; i < adj[node].size(); ++i) {
                int v = adj[node][i].to;
                int capacity = adj[node][i].capacity - adj[node][i].flow;
                if (!visited[v] && capacity > 0) {
                    parent[v] = node;
                    if (v == t) {
                        return true;
                    }
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        return false;
    }

    int fordFulkerson(int s, int t) {
        vector<int> parent(V, -1);
        int maxFlow = 0;

        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                for (Edge& edge : adj[u]) {
                    if (edge.to == v) {
                        pathFlow = min(pathFlow, edge.capacity - edge.flow);
                        break;
                    }
                }
            }

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                for (Edge& edge : adj[u]) {
                    if (edge.to == v) {
                        edge.flow += pathFlow;
                        break;
                    }
                }
                for (Edge& edge : adj[v]) {
                    if (edge.to == u) {
                        edge.flow -= pathFlow;
                        break;
                    }
                }
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    void printFlow(ofstream& outFile) {
        for (int u = 0; u < V; ++u) {
            for (Edge& edge : adj[u]) {
                if (edge.capacity > 0 && edge.flow > 0) {
                    outFile << "(" << u << ", " << edge.to << ", " << edge.flow << ")\n";
                }
            }
        }
    }

    pair<int, int> findSourceSink() {
        int source = -1, sink = -1;
        for (int i = 0; i < V; ++i) {
            // Ищем вершину с наибольшим количеством исходящих рёбер (предположительно исходная)
            if (out_degree[i] > 0 && in_degree[i] == 0) {
                source = i;
            }
            // Ищем вершину с наибольшим количеством входящих рёбер (предположительно сток)
            if (in_degree[i] > 0 && out_degree[i] == 0) {
                sink = i;
            }
        }
        return {source, sink};
    }
};

Graph readGraph(const string& filename) {
    ifstream inFile(filename);
    int V, u, v, capacity;
    inFile >> V;

    Graph graph(V);
    while (inFile >> u >> v >> capacity) {
        graph.addEdge(u, v, capacity);
    }

    return graph;
}

void writeResults(const string& outputFile, int maxFlow, Graph& graph) {
    ofstream outFile(outputFile);
    outFile << "Величина максимального потока: " << maxFlow << endl;
    outFile << "Потоки:" << endl;
    graph.printFlow(outFile);
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

    Graph graph = readGraph(FILE1);

    // Находим исходную и стоковую вершины
    pair<int, int> sourceSink = graph.findSourceSink();
    int source = sourceSink.first;
    int sink = sourceSink.second;

    if (source == -1 || sink == -1) {
        cout << "Source or Sink vertex not found!" << endl;
        return 1;
    }

    // Выполняем алгоритм Форда-Фалкерсона
    int maxFlow = graph.fordFulkerson(source, sink);
    writeResults(FILE2, maxFlow, graph);

    return 0;
}
