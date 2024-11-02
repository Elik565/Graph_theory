#include <iostream>
#include <limits>

#define INF std::numeric_limits<__int16_t>::max()

///// ПРОТОТИПЫ ФУНКЦИЙ /////

// Печать матрицы смежности в консоль
void print_matrix(__int16_t** matrix, const __int16_t N);

// Удаление матрицы смежности из памяти
void delete_matrix(__int16_t** matrix, const __int16_t N);

// Алгоритм Дейкстры
void Dijkstra(__int16_t** matrix, const __int16_t N, const int vertex, int* distances);

// Алгоритм Беллмана-Форда
bool Bellman_Ford(__int16_t** matrix, const __int16_t N, const int vertex, int* distances);

// Алгоритм Джонсона
void Johnson(__int16_t** matrix, const __int16_t N, std::ofstream& fout, const int vertex);