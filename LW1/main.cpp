#include <iostream>
#include <fstream>
#include "algorithms.hpp"

int main(int argc, char* argv[]) {
    // проверка входных параметров
    if (argc < 2) {
        std::cerr << "Программе не передано имя входного файла!\n";
        return 1;
    }

    if (argc == 3) {
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

    __int16_t N;
    fin >> N;

    // создание матрицы смежности
    __int16_t** matrix = new __int16_t*[N];
    for (int i = 0; i < N; i++) {
        matrix[i] = new __int16_t[N];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fin >> matrix[i][j];
            if (matrix[i][j] == 0) {
                matrix[i][j] = INF;
            }
        }
    }
    
    std::ofstream fout(FILE2);  // открытие выходного файла

    // печать исходной матрицы смежности
    std::cout << "Исходная матрица смежности:\n";
    print_matrix(matrix, N);

    // применяем алгоритм Беллмана-Форда
    int vertex = 0;
    fout << "Результат работы алгоритма Беллмана-Форда:\n";
    int distances[N];
    bool good_graph = Bellman_Ford(matrix, N, vertex, distances);

    // печать результата в файл
    if (good_graph == false) {
        fout << "Граф содержит цикл с отрицательным весом.\n";
        return 0;
    }

    fout << "Кратчайшие расстояния от вершины " << vertex << ":\n";
    for (int i = 0; i < N; i++) {
        if (i != vertex) {
            if (distances[i] == INF) {
                fout << "INF ";
            } else {
                fout << distances[i] << " ";
            }
        }
    }
    fout << "\n\n";
    
    // Применяем алгоритм Джонсона
    fout << "Результат работы алгоритма Джонсона:\n";
    Johnson(matrix, N, fout, 0);
    
    delete_matrix(matrix, N);

    return 0;
}
