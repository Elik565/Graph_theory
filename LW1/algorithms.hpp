#include <iostream>
#include <limits>

#define INF std::numeric_limits<int>::max()

///// ПРОТОТИПЫ ФУНКЦИЙ /////

// алгоритм Беллмана-Форда
bool Bellman_Ford(__int16_t** matrix, const __int16_t N, std::ofstream& fout, const int vertex);

void Johnson(__int16_t** matrix, const __int16_t N, std::ofstream& fout, const int vertex);