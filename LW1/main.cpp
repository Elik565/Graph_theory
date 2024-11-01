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

    int N;
    fin >> N;

    // создание матрицы смежности
    int** matrix = new int*[N];
    for (int i = 0; i < N; i++) {
        matrix[i] = new int[N];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fin >> matrix[i][j];
        }
    }

    std::ofstream fout(FILE2);  // открытие выходного файла

    Bellman_Ford(matrix, fout);

    return 0;
}
