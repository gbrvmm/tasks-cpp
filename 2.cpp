/*
Задание 2

Реализовать оберточный класс Graph, содержащий вершины графа и ребра между ними в виде элементов матрицы смежности.
Реализовать алгоритм обхода в глубину двумя способами:
• нерекурсивная реализация (с использованием стэка)
• рекурсивная реализация

При помощи этого алгоритма реализовать:
1) Проверку графа на двудольность;
2) Поиск цикла в ориентированном графе.
*/

#include <iostream>
#include <stack>

using namespace std;

const int max_n = 100; //максимальное количество вершин

class Graph {
    public:
        int adj[max_n][max_n]; //матрица смежности
        int n; //количество вершин

        //конструктор
        Graph(int _n) {
            n = _n;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    adj[i][j] = 0;
                }
            }
        }

        //добавление ребра
        void addEdge(int u, int v) {
            adj[u][v] = 1;
        }

        //проверка на двудольность (рекурсия)
        bool dvudol_recur(int u, int color[]) {
            if (color[u] != -1) { //вершина уже посещена
                for (int v = 0; v < n; v++) { //перебор соседей
                    if (adj[u][v] == 1 && (color[v] == -1 || (color[u] == 1 && color[v] != 2) || (color[u] == 2 && color[v] != 1))) {                
                        return color[u] == 1 ? color[v] == 2 : color[v] == 1; //проверка цвета соседней вершины
                    }

                    color[u] = 1; //покрасить вершину в цвет 1

                    for (int v = 0; v < n; v++) {
                        if (adj[u][v] == 1 && !dvudol_recur(v, color)) {
                            return false;
                        }
                    }
                    color[u] = 2; //покрасить вершину в цвет 2
                    return true;
                }
            }
        };

        //проверка на двудольность (стэк)
        bool dvudol_stack() {
            int color[max_n];
            for (int i = 0; i < n; i++) {
                color[i] = -1; //непосещенная вершина
            }

            stack<int> s;
            s.push(0);
            color[0] = 1; //покрасить вершину 0 в цвет 1

            while (!s.empty()) {
                int u = s.top();
                s.pop();

                for (int v = 0; v < n; v++) {
                    if (adj[u][v] == 1 && color[v] == -1) {
                        color[v] = (color[u] == 1) ? 2 : 1; //покрасить соседнюю вершину в другой цвет
                        s.push(v);
                    }
                    else if (adj[u][v] == 1 && color[u] == color[v]) {
                        return false; //обнаружен цикл одного цвета
                    }
                }
            }
            return true;
        }

        //поиск цикла в ориентированном графе (рекурсия)
        bool cycle_recur(int u, int color[]) {
            if (color[u] == 1) { //вершина в процессе обработки
                return true;
            }

            if (color[u] != -1) { //вершина уже обработана
                return false;
            }

            color[u] = 1; //покрасить вершину в цвет 1

            for (int v = 0; v < n; v++) {
                if (adj[u][v] == 1 && cycle_recur(v, color)) {
                    return true;
                }
            }

            color[u] = 2; //покрасить вершину в цвет 2
                return false;
        }

        //поиск цикла в ориентированном графе (стэк)
        bool cycle_stack() {
            int color[max_n];
            for (int i = 0; i < n; i++) {
                color[i] = -1; ///непосещенная вершина
            }

            stack<int> s;
            for (int i = 0; i < n; i++) {
                if (color[i] == -1) {
                    s.push(i);
                    color[i] = 1; //покрасить вершину в цвет 1

                    while (!s.empty()) {
                    int u = s.top();
                    s.pop();

                    for (int v = 0; v < n; v++) {
                        if (adj[u][v] == 1) {
                            if (color[v] == -1) { //непосещенная вершина
                                color[v] = (color[u] == 1) ? 2 : 1; //покрасить в другой цвет
                                s.push(v);
                            } else if (color[v] == color[u]) { //обнаружен цикл одного цвета
                                return true;
                            }
                        }
                    }
                }
            }
                return false;
            }
        }
};

int main() {
    cout << "Введите количество вершин графа: " << endl;
    int n; //количество вершин
    cin >> n;

    Graph graph(n); //создание графа

    cout << "Введите циферки матрицы смежности: " <<endl;
    //считывание ребер
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph.adj[i][j];
        }
    }

    //проверка на двудольность (рекурсивная)
    int color[max_n];
    for (int i = 0; i < n; i++) {
        color[i] = -1;
    }

    bool dvudol_recur = graph.dvudol_recur(0, color);

    if (dvudol_recur) {
        cout << "Граф является двудольным (рекурсия)" << endl;
        for (int i = 0; i < n; i++) {
            cout << "Вершина " << i << " имеет цвет " << color[i] << endl;
        }
    }
    else {
        cout << "Граф не является двудольным (рекурсия)" << endl;
    }

    //проверка на двудольность (стэк)
    dvudol_recur = graph.dvudol_stack();
    if (dvudol_recur) {
        cout << "Граф является двудольным (стэк)" << endl;
    }
    else {
        cout << "Граф не является двудольным (стэк)" << endl;
    }

    //поиск цикла в ориентированном графе (рекурсия)
    color[max_n];
    for (int i = 0; i < n; i++) {
        color[i] = -1;
    }

    bool cycle_recur = graph.cycle_recur(0, color);

    if (cycle_recur) {
        cout << "В графе есть цикл (рекурсия)" << endl;
    }
    else {
        cout << "В графе нет цикла (рекурсия)" << endl;
    }

    //поиск цикла в ориентированном графе (стэк)
    cycle_recur = graph.cycle_stack();

    if (cycle_recur) {
        cout << "В графе есть цикл (стэк)" << endl;
    }
    else {
        cout << "В графе нет цикла (стэк)" << endl;
    }

    return 0;
}
