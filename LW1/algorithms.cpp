#include "algorithms.hpp"
#include <fstream>

void print_matrix(__int16_t** matrix, const __int16_t N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == INF) {
                std::cout << "INF\t";
            } 
            else {
                std::cout << matrix[i][j] << "\t";
            }
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

void delete_matrix(__int16_t** matrix, const __int16_t N) {
    for (int i = 0; i < N; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

void Dijkstra(__int16_t** matrix, const __int16_t N, const int vertex, int* distances) {
    
}

bool Bellman_Ford(__int16_t** matrix, const __int16_t N, const int vertex, int* distances) {
    // заполняем массив расстояний
    for (int i = 0; i < N; i++) {
        distances[i] = INF;
    }
    distances[vertex] = 0;

    // основной процесс
    for (int k = 0; k < N - 1; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (matrix[i][j] != INF && distances[i] != INF && (matrix[i][j] + distances[i]) < distances[j]) {
                    distances[j] = distances[i] + matrix[i][j];
                }
            }
        }
    }

    // проверка на цикл с отрицательным весом
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] != INF && distances[i] != INF && distances[j] > matrix[i][j] + distances[i]) { 
                return false;  // если смогли уменьшить расстояние
            }
        }
    }

    return true;
}   

void Johnson(__int16_t** matrix, const __int16_t N, std::ofstream& fout, const int vertex) {
    // создаем новую матрицу смежности с дополнительной вершиной
    int new_size = N + 1;
    __int16_t** new_matrix = new __int16_t*[N + 1];
    for (int i = 0; i < new_size; i++) {
        new_matrix[i] = new __int16_t[new_size];
    }
    
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            if (j == N) {
                new_matrix[i][j] = INF;
            }
            else if (i == N) {
                new_matrix[i][j] = 0;
            }
            else {
                new_matrix[i][j] = matrix[i][j];
            }
        }
    }

    std::cout << "Матрица смежности для алгоритма Джонсона:\n";
    print_matrix(new_matrix, new_size);

    // применяем алгоритм Беллмана-Форда
    int distances[new_size];
    bool good_graph = Bellman_Ford(new_matrix, new_size, new_size - 1, distances);

    // проверка на наличие цикла с отрицательным весом
    if (good_graph == false) {
        delete_matrix(new_matrix, new_size);
        return;
    }

    // перерасчет весов
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            if (new_matrix[i][j] != INF) {
                new_matrix[i][j] = new_matrix[i][j] + distances[i] - distances[j];
            }
        }
    }

    delete[] new_matrix[N];  // удаление дополнительной вершины

    std::cout << "Матрица смежности с пересчитанными весами:\n";
    print_matrix(new_matrix, N);



    return;
}