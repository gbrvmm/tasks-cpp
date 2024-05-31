/*
Задание 7

В населённом пункте присутствует N остановок. Дана информация о транспортных маршрутах в виде последовательного списка
остановок через которые проходит маршрут.

Задание 0. Разработать наиболее удобный формат входных данных для этой задачи.

Задание 1. По заданной входной информации проверить возможность попасть из одной остановки на другую на одном транспорте.
Проверить возможность попасть из одной остановки на другую с заданным количеством пересадок.

Задание 2. Проверить Возможно ли с каждой остановки добраться до любой другой остановки.

Задание 3. Проверить выполнение предыдущего условия при удалении любого из маршрутов

Задание 4. Найти минимальное количество маршрутов удаление которых приведёт к прекращению выполнения условия задания 2. 

Задание 5. Разработать не менее 6 различных тестов для проверки корректности работы всех алгоритмов. 
*/


#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

const int max_stops = 100; //максимальное количество остановок

struct Stop {
    int stop_id; //идентификатор остановки
};

bool matrix[max_stops][max_stops]; //матрица смежности для маршрутов

//проверка возможности попасть из одной остановки в другую на одном транспорте
bool odna_peresadka(Stop* stops, int numStops, int from, int to) {
    return matrix[from][to];
}

//проверка возможности попасть из одной остановки в другую с заданным количеством пересадок
bool n_peresadki(Stop* stops, int numStops, int from, int to, int n) {
    queue<pair<int, int>> q; //очередь для хранения пары (текущая остановка, количество пересадок)
    bool visited[max_stops];
    memset(visited, false, sizeof(visited));

    q.push({from, 0});
    visited[from] = true;

    while (!q.empty()) {
        auto [currentStop, transfers] = q.front();
        q.pop();

        if (currentStop == to && transfers <= n) return true;
        if (transfers < n) {
            for (int i = 0; i < numStops; ++i) {
                if (matrix[currentStop][i] && !visited[i]) {
                    q.push({i, transfers + 1});
                    visited[i] = true;
                }
            }
        }
    }
    return false;
}

//проверка возможности добраться из каждой остановки до любой другой
bool all_stops(Stop* stops, int numStops) {
    for (int i = 0; i < numStops; ++i) {
        bool visited[max_stops];
        memset(visited, false, sizeof(visited));

        queue<int> q;
        q.push(i);
        visited[i] = true;

        int count = 0;
        while (!q.empty()) {
            int currentStop = q.front();
            q.pop();
            count++;

            for (int j = 0; j < numStops; ++j) {
                if (matrix[currentStop][j] && !visited[j]) {
                    q.push(j);
                    visited[j] = true;
                }
            }
        }
        if (count != numStops) return false;
    }
    return true;
}

//проверка выполнения условия задания 2 при удалении маршрута с индексом routeIndex
bool del_puti(Stop* stops, int numStops, int routeIndex) {
    //сохраняем текущий маршрут
    bool temp[max_stops][max_stops];
    memcpy(temp, matrix, sizeof(matrix));

    //удаляем маршрут
    for (int i = 0; i < numStops; ++i) {
        matrix[routeIndex][i] = matrix[i][routeIndex] = false;
    }

    bool result = all_stops(stops, numStops);

    //восстанавливаем маршрут
    memcpy(matrix, temp, sizeof(matrix));
    return result;
}

// нахождение минимального количества маршрутов, удаление которых приведет к нарушению условия задания 2
int min_puti(Stop* stops, int numStops) {
    int minRemovals = max_stops;

    for (int i = 0; i < numStops; ++i) {
        //сохраняем текущий маршрут
        bool temp[max_stops][max_stops];
        memcpy(temp, matrix, sizeof(matrix));

        //удаляем маршрут
        for (int j = 0; j < numStops; ++j) {
            matrix[i][j] = matrix[j][i] = false;
        }

        if (!all_stops(stops, numStops)) {
            int removals = 0;
            for (int j = 0; j < numStops; ++j) {
                if (!temp[i][j]) removals++;
            }
            minRemovals = min(minRemovals, removals);
        }

        //восстанавливаем маршрут
        memcpy(matrix, temp, sizeof(matrix));
    }
    return minRemovals;
}

void resetmatrix(int numStops) {
    for (int i = 0; i < numStops; ++i) {
        for (int j = 0; j < numStops; ++j) {
            matrix[i][j] = false;
        }
    }
}

int main() {
    //входные данные
    int numStops = 5;
    Stop stops[] = {{0}, {1}, {2}, {3}, {4}};

    //матрица смежности
    memset(matrix, false, sizeof(matrix));
    matrix[0][1] = matrix[1][0] = true;
    matrix[1][2] = matrix[2][1] = true;
    matrix[2][3] = matrix[3][2] = true;
    matrix[3][4] = matrix[4][3] = true;

    //тест 1
    resetmatrix(numStops);
    matrix[0][1] = matrix[1][0] = true;
    matrix[1][2] = matrix[2][1] = true;
    matrix[2][3] = matrix[3][2] = true;
    matrix[3][4] = matrix[4][3] = true;
    cout << "Тест 1" << endl;
    cout << "Можно добраться с одной пересадкой: " << odna_peresadka(stops, numStops, 0, 1) << endl; 
    cout << "Можно добраться с 3 пересадками: " << n_peresadki(stops, numStops, 0, 4, 3) << endl;
    cout << "Можно доехать до всех остановок: " << all_stops(stops, numStops) << endl;
    cout << "Проверка удаления маршрута (2): " << del_puti(stops, numStops, 2) << endl;
    cout << "Минимальные маршруты для удаления: " << min_puti(stops, numStops) << endl;
    
    //тест 2
    resetmatrix(numStops);
    for (int i = 0; i < numStops; ++i) {
        for (int j = 0; j < numStops; ++j) {
            if (i != j) {
                matrix[i][j] = true;
            }
        }
    }
    cout << "Тест 2" << endl;
    cout << "Можно добраться с одной пересадкой: " << odna_peresadka(stops, numStops, 0, 4) << endl; 
    cout << "Можно добраться с одной пересадкой: " << n_peresadki(stops, numStops, 0, 4, 1) << endl;
    cout << "Можно доехать до всех остановок: " << all_stops(stops, numStops) << endl;
    cout << "Проверка удаления маршрута (2): " << del_puti(stops, numStops, 2) << endl; 
    cout << "Минимальные маршруты для удаления: " << min_puti(stops, numStops) << endl; 

    //тест 3
    resetmatrix(numStops);
    matrix[0][1] = matrix[1][0] = true;
    matrix[1][2] = matrix[2][1] = true;
    matrix[2][3] = matrix[3][2] = true;
    cout << "Тест 3" << endl;
    cout << "Можно добраться с одной пересадкой: " << odna_peresadka(stops, numStops, 0, 4) << endl; // Expected: 0
    cout << "Можно добраться с тремя пересадками: " << n_peresadki(stops, numStops, 0, 3, 2) << endl; // Expected: 1
    cout << "Можно доехать до всех остановок: " << all_stops(stops, numStops) << endl; // Expected: 0
    cout << "Проверка удаления маршрута (2): " << del_puti(stops, numStops, 2) << endl; // Expected: 0
    cout << "Минимальные маршруты для удаления: " << min_puti(stops, numStops) << endl; // Expected: 1

    //тест 4
    resetmatrix(numStops);
    matrix[0][1] = matrix[1][0] = true;
    matrix[1][2] = matrix[2][1] = true;
    matrix[2][3] = matrix[3][2] = true;
    matrix[3][4] = matrix[4][3] = true;
    matrix[4][0] = matrix[0][4] = true;
    cout << "Тест 4" << endl;
    cout << "Можно добраться с одной пересадкой: " << odna_peresadka(stops, numStops, 0, 4) << endl;
    cout << "Можно добраться с двумя пересадками: " << n_peresadki(stops, numStops, 0, 2, 2) << endl;
    cout << "Можно доехать до всех остановок: " << all_stops(stops, numStops) << endl;
    cout << "Проверка удаления маршрута (2): " << del_puti(stops, numStops, 2) << endl;
    cout << "Минимальные маршруты для удаления: " << min_puti(stops, numStops) << endl;

    //тест 5
    resetmatrix(numStops);
    matrix[0][1] = matrix[1][0] = true;
    matrix[0][2] = matrix[2][0] = true;
    matrix[1][3] = matrix[3][1] = true;
    matrix[1][4] = matrix[4][1] = true;
    cout << "Тест 5" << endl;
    cout << "Можно добраться с одной пересадкой: " << odna_peresadka(stops, numStops, 0, 4) << endl;
    cout << "Можно добраться с двумя пересадками: " << n_peresadki(stops, numStops, 0, 4, 2) << endl;
    cout << "Можно доехать до всех остановок: " << all_stops(stops, numStops) << endl;
    cout << "Route removal check (1): " << del_puti(stops, numStops, 1) << endl;
    cout << "Минимальные маршруты для удаления: " << min_puti(stops, numStops) << endl;

    //тест 6
    resetmatrix(numStops);
    matrix[0][1] = matrix[1][0] = true;
    matrix[2][3] = matrix[3][2] = true;
    matrix[3][4] = matrix[4][3] = true;
    cout << "Тест 6" << endl;
    cout << "Можно добраться с одной пересадкой: " << odna_peresadka(stops, numStops, 0, 2) << endl; 
    cout << "Можно добраться с двумя пересадками: " << n_peresadki(stops, numStops, 0, 2, 2) << endl; 
    cout << "Можно доехать до всех остановок: " << all_stops(stops, numStops) << endl; 
    cout << "Route removal check (3): " << del_puti(stops, numStops, 3) << endl; 
    cout << "Минимальные маршруты для удаления: " << min_puti(stops, numStops) << endl; 

    return 0;
}
