#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>
#include <limits>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max();

// Направления для движения (вверх, вниз, влево, вправо)
const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

struct Node {
    int x, y;
    int cost;
    int heuristic;
    Node(int x, int y, int cost, int heuristic = 0) : x(x), y(y), cost(cost), heuristic(heuristic) {}

    bool operator>(const Node& other) const {
        return cost + heuristic > other.cost + other.heuristic;  // Для приоритетной очереди
    }
};

// Функция для чтения карты высот из файла
vector<vector<int>> readMap(const string& filename, int& rows, int& cols) {
    ifstream file(filename);
    file >> rows >> cols;
    vector<vector<int>> map(rows, vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> map[i][j];
        }
    }

    return map;
}

// Эвристики

// Эвклидова эвристика
int euclideanHeuristic(int x1, int y1, int x2, int y2) {
    return static_cast<int>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

// Манхэттенская эвристика
int manhattanHeuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

// Эвристика Чебышева
int chebyshevHeuristic(int x1, int y1, int x2, int y2) {
    return max(abs(x2 - x1), abs(y2 - y1));
}

// Алгоритм Дейкстры
vector<pair<int, int>> dijkstra(const vector<vector<int>>& map, int startX, int startY, int endX, int endY, int& visitedNodes) {
    int rows = map.size(), cols = map[0].size();
    vector<vector<int>> dist(rows, vector<int>(cols, INF));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[startX][startY] = 0;
    pq.push(Node(startX, startY, 0));

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        int x = current.x, y = current.y;

        if (x == endX && y == endY) break; // Найден путь до конца

        if (dist[x][y] < current.cost) continue; // Пропускаем уже обработанные вершины

        visitedNodes++;

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                int newDist = dist[x][y] + abs(map[x][y] - map[nx][ny]);
                if (newDist < dist[nx][ny]) {
                    dist[nx][ny] = newDist;
                    parent[nx][ny] = {x, y};
                    pq.push(Node(nx, ny, newDist));
                }
            }
        }
    }

    vector<pair<int, int>> path;
    for (int x = endX, y = endY; x != startX || y != startY; tie(x, y) = parent[x][y]) {
        path.push_back({x, y});
    }
    path.push_back({startX, startY});
    reverse(path.begin(), path.end());

    return path;
}

// Алгоритм A* с различными эвристиками
vector<pair<int, int>> aStar(const vector<vector<int>>& map, int startX, int startY, int endX, int endY, int& visitedNodes, int(*heuristic)(int, int, int, int)) {
    int rows = map.size(), cols = map[0].size();
    vector<vector<int>> dist(rows, vector<int>(cols, INF));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[startX][startY] = 0;
    pq.push(Node(startX, startY, 0, heuristic(startX, startY, endX, endY)));

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        int x = current.x, y = current.y;

        if (x == endX && y == endY) break; // Найден путь до конца

        if (dist[x][y] < current.cost) continue; // Пропускаем уже обработанные вершины

        visitedNodes++;

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                int newDist = dist[x][y] + abs(map[x][y] - map[nx][ny]);
                int newHeuristic = heuristic(nx, ny, endX, endY);
                if (newDist + newHeuristic < dist[nx][ny] + newHeuristic) {
                    dist[nx][ny] = newDist;
                    parent[nx][ny] = {x, y};
                    pq.push(Node(nx, ny, newDist, newHeuristic));
                }
            }
        }
    }

    vector<pair<int, int>> path;
    for (int x = endX, y = endY; x != startX || y != startY; tie(x, y) = parent[x][y]) {
        path.push_back({x, y});
    }
    path.push_back({startX, startY});
    reverse(path.begin(), path.end());

    return path;
}

// Запись результатов в файл
void writeResults(const string& outputFile, const string& algo, int pathLength, int visitedNodes, double duration, const vector<pair<int, int>>& path) {
    ofstream outFile(outputFile, ios::app);
    outFile << algo << ":\n";
    outFile << "Path length: " << pathLength << "\n";
    outFile << "Visited nodes: " << visitedNodes << "\n";
    outFile << "Execution time: " << duration << " seconds\n";
    outFile << "Path: ";
    for (const auto& p : path) {
        outFile << "(" << p.first << ", " << p.second << ") ";
    }
    outFile << "\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 9) {
        cout << "Usage: ./pathfinding <input_file> -s xstart ystart -e xend yend [-o output_file]\n";
        return 1;
    }

    string inputFile = argv[1];
    int startX = stoi(argv[3]), startY = stoi(argv[4]);
    int endX = stoi(argv[6]), endY = stoi(argv[7]);
    string outputFile = "output.txt";

    if (argc > 8 && string(argv[8]) == "-o") {
        outputFile = argv[9];
    }

    int rows, cols;
    vector<vector<int>> map = readMap(inputFile, rows, cols);

    auto start = chrono::high_resolution_clock::now();
    int visitedNodes = 0;
    vector<pair<int, int>> dijkstraPath = dijkstra(map, startX, startY, endX, endY, visitedNodes);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    writeResults(outputFile, "Dijkstra", dijkstraPath.size(), visitedNodes, duration.count(), dijkstraPath);

    visitedNodes = 0;
    start = chrono::high_resolution_clock::now();
    vector<pair<int, int>> aStarEuclidean = aStar(map, startX, startY, endX, endY, visitedNodes, euclideanHeuristic);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    writeResults(outputFile, "A* (Euclidean)", aStarEuclidean.size(), visitedNodes, duration.count(), aStarEuclidean);

    visitedNodes = 0;
    start = chrono::high_resolution_clock::now();
    vector<pair<int, int>> aStarManhattan = aStar(map, startX, startY, endX, endY, visitedNodes, manhattanHeuristic);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    writeResults(outputFile, "A* (Manhattan)", aStarManhattan.size(), visitedNodes, duration.count(), aStarManhattan);

    visitedNodes = 0;
    start = chrono::high_resolution_clock::now();
    vector<pair<int, int>> aStarChebyshev = aStar(map, startX, startY, endX, endY, visitedNodes, chebyshevHeuristic);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    writeResults(outputFile, "A* (Chebyshev)", aStarChebyshev.size(), visitedNodes, duration.count(), aStarChebyshev);

    return 0;
}
