#include <iostream>
#include <limits>

#define INF std::numeric_limits<__int16_t>::max()

///// ПРОТОТИПЫ ФУНКЦИЙ /////

// Печать матрицы смежности в консоль
void print_matrix(__int16_t** matrix, const __int16_t N);

// Удаление матрицы смежности из памяти
void delete_matrix(__int16_t** matrix, const __int16_t N);

// Алгоритм Дейкстры
void Dijkstra(__int16_t** new_matrix, __int16_t** matrix, __int16_t N, const int vertex, __int16_t* distances);

// Алгоритм Беллмана-Форда
bool Bellman_Ford(__int16_t** matrix, const __int16_t N, const int vertex, __int16_t* distances);

// Алгоритм Джонсона
void Johnson(__int16_t** matrix, const __int16_t N, __int16_t** dist_matrix);

// Поиск диаметра и соответствующих ему вершин 
void find_diameter(__int16_t** dist_matrix, const __int16_t N, __int16_t* diameter);

// Поиск радиуса и соответствующих ему вершин
void find_radius(__int16_t** dist_matrix, const __int16_t N, __int16_t* radius);

// Поиск множества центральных вершин
int find_central_vertices(__int16_t** dist_matrix, const __int16_t N, const __int16_t radius, int* vertices);

// Поиск множества периферийных вершин
int find_peripheral_vertices(__int16_t** dist_matrix, const __int16_t N, const __int16_t diameter, int* vertices);