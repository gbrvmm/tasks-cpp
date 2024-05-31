/*
Задание 1

Реализовать оберточный класс Graph, содержащий вершины графа и ребра между ними в виде элементов матрицы смежности. 
Реализовать алгоритм обхода в ширину.
При помощи этого алгоритма реализовать решение следующих задач:
• Поиск кратчайшего пути между двумя заданными точками;
• Нахождение кратчайшего цикла в ориентированном невзвешенном графе;
• Поиск компонент связности в графе;
• Найти вершину (или несколько) удаление которой разбивает граф несколько компонент связности.
*/

#include <iostream>
#include <queue>

using namespace std;

const int max_n = 100; //максимальное число вершин

class Graph {
public:
    int n; //число вершин
    bool adj[max_n][max_n]; //матрица смежности

    //конструктор
    Graph(int _n) {
        n = _n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                adj[i][j] = false;
            }
        }
    }

    //добавление ребра
    void addEdge(int u, int v) {
        adj[u][v] = true;
        adj[v][u] = true; //неориентированный граф
    }

    void removeEdge(int u, int v) {
        if (u < 0 || u >= n || v < 0 || v >= n) {
            //обработка ошибки: неверные индексы вершин
            return;
        }

        adj[u][v] = false; //удаление ребра из матрицы смежности
        adj[v][u] = false; //неориентированный граф

        // обновление информации о степени вершин
        // degree[u]--;
        // degree[v]--;
    }
    void removePoint(int u) {
        for (int i = 0; i < n; i++) {
            adj[i][u] = false;
            
        }
        for (int i = 0; i < n; i++) {
            adj[u][i] = false; 

        }
    }
};

//функция обхода в ширину
void bfs(Graph& graph, int s, int* d, int* p, bool* used) {
    queue<int> q;
    q.push(s);
    used[s] = true;
    d[s] = 0;
    p[s] = -1; //нет предка

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u = 0; u < graph.n; u++) {
            if (graph.adj[v][u] && !used[u]) {
                used[u] = true;
                d[u] = d[v] + 1;
                p[u] = v;
                q.push(u);
            }
        }
    }
}

//поиск кратчайшего пути между двумя вершинами
void findShortestPath(Graph& graph, int s, int t) {
    int d[max_n], p[max_n];
    bool* used = new bool[max_n];
    for (int i = 0; i < graph.n; i++) {
        d[i] = INT_MAX;
        p[i] = -1;
        used[i] = false;
    }

    bfs(graph, s, d, p, used);

    if (used[t]) {
        //путь существует
        vector<int> path;
        int v = t;
        while (v != s) {
            path.push_back(v);
            v = p[v];
        }
        path.push_back(s);
        reverse(path.begin(), path.end());

        cout << "Путь: ";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << " ";
        }
        cout << endl;
    }
    else {
        cout << "Пути между вершинами " << s << " и " << t << " нет" << endl;
    }
}

//поиск кратчайшего цикла в ориентированном графе
void findShortestCycle(Graph& graph) {
    int d[max_n], p[max_n];
    bool* used = new bool[max_n];
    int shortest_cycle_length = INT_MAX;
    int shortest_cycle_start = -1;

    for (int i = 0; i < graph.n; i++) {
        for (int j = 0; j < graph.n; j++) {
            used[j] = false;
        }
        
        bfs(graph, i, d, p, used);
       
        for (int v = 0; v < graph.n; v++) {
            
            if (used[v] && graph.adj[v][i]) { //найден цикл
                int cycle_length = d[v] + 1;
                if (cycle_length < shortest_cycle_length) {
                    shortest_cycle_length = cycle_length;
                    shortest_cycle_start = i;
                }
            }
        }
        
    }

    if (shortest_cycle_length != INT_MAX) {
        //вывод кратчайшего цикла
        vector<int> cycle;
        int v = shortest_cycle_start;
        cout << v << endl;
        for (int i = 0; i < 4; i++) {
            cout << p[i] << " ";
        }
        do {
            cycle.push_back(v);
            v = p[v];
        } while (v != shortest_cycle_start);
        cycle.push_back(shortest_cycle_start);

        cout << "Кратчайший цикл: ";
        for (int i = 0; i < cycle.size(); i++) {
            cout << cycle[i] << " ";
        }
        cout << endl;
    }
    else {
        cout << "В графе нет циклов" << endl;
    }
}

//поиск компонент связности в графе
int findComponents(Graph& graph) {
    int components[max_n];
    int num_components = 0;

    for (int i = 0; i < graph.n; i++) {
        components[i] = -1; //вершина не принадлежит ни одной компоненте
    }
;    for (int i = 0; i < graph.n; i++) {
        if (components[i] == -1) {
            num_components++;
            int* d = new int[graph.n];
            int* p = new int [graph.n];
            bool* used = new bool[max_n];
            for (int i = 0; i < graph.n; i++) {
                d[i] = INT_MAX;
                p[i] = -1;
                used[i] = false;
            }
            bfs(graph, i, d, p, used);
            for (int i = 0; i < graph.n; i++) {
                if (used[i])
                components[i] = num_components;
            }
        }
    }

    cout << "Число компонент связности: " << num_components << endl;
    for (int i = 0; i < graph.n; i++) {
        cout << "Вершина " << i << " принадлежит компоненте " << components[i] + 1 << endl;
    }
    return num_components;
}

//найти вершину (или несколько), удаление которой разбивает граф на несколько компонент связности
void findVertexToBreakComponents(Graph& graph) {
    int nc = findComponents(graph);
    for (int i = 0; i < graph.n; i++) {
        Graph temp_graph = graph; //копия графа
        temp_graph.removePoint(i); //удаление i-ой вершины
        int nc1 = findComponents(temp_graph);

        if (nc1 > nc) {
            cout << "Удаление вершины " << i << " разбивает граф на " << nc1 << " компоненты связности" << endl;
            return;
        }
    }

    cout << "Нет вершины, удаление которой разбивает граф на несколько компонент связности" << endl;
}

int main() {
    //создаем граф
    setlocale(LC_ALL, "ru");
    Graph graph(4);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);

    //поиск кратчайшего пути
    findShortestPath(graph, 0, 3);

    //поиск кратчайшего цикла
    //findShortestCycle(graph);
    cout << "1231242134213" << endl;
    //поиск компонент связности
    findComponents(graph);
    cout << "1231242134213" << endl;
    //поиск вершины, удаление которой разбивает граф на несколько компонент связности
    findVertexToBreakComponents(graph);

    return 0;
}
