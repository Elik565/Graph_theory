#include <iostream>
#include <fstream>
#include "algorithms.hpp"

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
        }
    }
    
    std::ofstream fout(FILE2);  // открытие выходного файла

    // печать исходной матрицы смежности
    std::cout << "Исходная матрица смежности:\n";
    print_matrix(matrix, N);

    // применяем алгоритм Беллмана-Форда
    int vertex = 0;
    fout << "Результат работы алгоритма Беллмана-Форда:\n";
    __int16_t distances[N];
    bool good_graph = Bellman_Ford(matrix, N, vertex, distances);

    // печать результата в файл
    if (good_graph == false) {
        fout << "Граф содержит цикл с отрицательным весом.\n";
        return 0;
    }

    fout << "Кратчайшие расстояния от вершины " << vertex << ":\n";
    for (int i = 0; i < N; i++) {
        if (distances[i] == INF) {
            fout << "0 ";
        } 
        else {
            fout << distances[i] << " ";
        }
    }
    fout << "\n\n";
    
    // применяем алгоритм Джонсона
    __int16_t** dist_matrix = new __int16_t*[N];
    for (int i = 0; i < N; i++) {
        dist_matrix[i] = new __int16_t[N];
    }

    Johnson(matrix, N, dist_matrix);
    
    // печать результата в файл
    fout << "Результат работы алгоритма Джонсона:\n";
    fout << "Матрица расстояний:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (dist_matrix[i][j] == INF) {
                dist_matrix[i][j] = 0;
            }
            
            fout << dist_matrix[i][j] << "\t";
        }

        fout << "\n";
    }
    fout << "\n";

    // поиск диаметра графа
    __int16_t diameter[3] = {0};
    find_diameter(dist_matrix, N, diameter);
    fout << "Диаметр графа = " << diameter[0] << " от вершины " << diameter[1] << " до вершины " << diameter[2];
    fout << ".\n";

    // поиск радиуса графа
    __int16_t radius[3] = {0};
    find_radius(dist_matrix, N, radius);
    fout << "Радиус графа = " << radius[0] << " от вершины " << radius[1] << " до вершины " << radius[2];
    fout << ".\n";

    // поиск множества центральных вершин
    int vertices[N];
    int count = find_central_vertices(dist_matrix, N, radius[0], vertices);
    fout << "Множество центральных вершин: ";
    for (int i = 0; i < count; i++) {
        fout << vertices[i] << " ";
    }
    fout << "\n";

    // завершение программы
    delete_matrix(matrix, N);
    fout.close();

    return 0;
}
