#include "algorithms.hpp"
#include <fstream>

void print_matrix(__int16_t** matrix, const __int16_t N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == INF) {
                std::cout << "0\t";
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

void Dijkstra(__int16_t** new_matrix, __int16_t** matrix, __int16_t N, const int vertex, __int16_t* distances) {
    // заполняем массив расстояний
    for (int i = 0; i < N; i++) {
        distances[i] = INF;
    }
    distances[vertex] = 0;

    bool visited[N] = {false};  //  массив посещенных вершин

    for (int i = 0; i < N - 1; i++) {
        __int16_t min = INF;
        int u;

        for (int v = 0; v < N; v++)
        {
            if (!visited[v] && distances[v] <= min)
            {
                min = distances[v];
                u = v;
            }
        }

        visited[u] = true;

        for (int k = 0; k < N; k++) {
            if (matrix[u][k] != INF && !visited[k] && distances[u] != INF && distances[u] + new_matrix[u][k] < distances[k]) {
                distances[k] = distances[u] + new_matrix[u][k];
            }
        }
    }
}

bool Bellman_Ford(__int16_t** matrix, const __int16_t N, const int vertex, __int16_t* distances) {
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
 
void Johnson(__int16_t** matrix, const __int16_t N, __int16_t** distance_table) {
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
    __int16_t distances[new_size];
    bool good_graph = Bellman_Ford(new_matrix, new_size, new_size - 1, distances);

    // проверка на наличие цикла с отрицательным весом
    if (good_graph == false) {
        delete_matrix(new_matrix, new_size);
        return;
    }

    // перерасчет весов
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (new_matrix[i][j] != INF) {
                new_matrix[i][j] = new_matrix[i][j] + distances[i] - distances[j];
            }
        }
    }

    std::cout << "Матрица смежности с пересчитанными весами:\n";
    print_matrix(new_matrix, N);

    // находим кратчайшие расстояния от каждой вершины
    for (int i = 0; i < N; i++) {
        Dijkstra(new_matrix, matrix, N, i, distance_table[i]);
    }

    // восстанавливаем изначальные расстояния
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (distance_table[i][j] == INF) 
                continue;

            distance_table[i][j] += distances[j] - distances[i];
        }
    }

    delete_matrix(new_matrix, new_size);  // удаляем созданную матрицу

    return;
}

void find_diameter(__int16_t** dist_matrix, const __int16_t N, __int16_t* diameter) {
    diameter[0] = dist_matrix[0][0];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (dist_matrix[i][j] > diameter[0]) {
                diameter[0] = dist_matrix[i][j];
                diameter[1] = i;
                diameter[2] = j;
            }
        }
    }

    return;
}