#include "algorithms.hpp"
#include <fstream>

bool Bellman_Ford(__int16_t** matrix, const __int16_t N, std::ofstream& fout, const int vertex) {
    // заполняем массив расстояний
    int distance[N];
    for (int i = 0; i < N; i++) {
        distance[i] = INF;
    }
    distance[vertex] = 0;

    // основной процесс
    for (int k = 0; k < N - 1; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (matrix[i][j] != 0 && distance[i] != INF && (matrix[i][j] + distance[i]) < distance[j]) {
                    distance[j] = distance[i] + matrix[i][j];
                }
            }
        }
    }

    // проверка на отрицательный цикл
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] != 0 && distance[i] != INF && distance[j] > matrix[i][j] + distance[i]) { 
                fout << "Граф содержит отрицательный цикл.\n\n";  // если смогли уменьшить расстояние
                return false;
            }
        }
    }

    fout << "Кратчайшие расстояния от вершины " << vertex << ":\n";
    for (int i = 0; i < N; i++) {
        if (i != vertex) {
            if (distance[i] == INF) {
                fout << "INF ";
            } else {
                fout << distance[i] << " ";
            }
        }
    }
    fout << "\n\n";

    return true;
}   

void Johnson(__int16_t** matrix, const __int16_t N, std::ofstream& fout, const int vertex) {

}