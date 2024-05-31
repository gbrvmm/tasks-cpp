/*
Задание 3

Реализовать оберточный класс Graph, содержащий вершины графа и ребра между ними в виде элементов матрицы смежности.
Реализовать алгоритм Дейкстры. При помощи этого алгоритма реализовать решение следующих задач.
На вход подается список городов, расстояния между ними, время и стоимость перелета между парами городов разными рейсами разных авиакомпаний.
- Реализовать поиск маршрутов между несовместными городами (не имеющими прямых рейсов) по минимальному времени перелета, по минимальной стоимости перелета.
- Реализовать поиск списка городов, в которые можно попасть из заданного, имея определенную сумму денег.
- Реализовать поиск списка городов, в которые можно попасть за определенное время.
- Определить, можно ли попасть из одного города в другой не более чем с двумя или тремя пересадками.
- Определить максимальное расстояние, на которое можно улететь с пересадками, имея определенную сумму.
*/

#include <iostream>
#include <string>
#include <climits>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

//структура для представления вершины графа
struct Node {
    int id; //номер вершины
    string name; //название вершины (город)
    int distance; //расстояние до вершины (время/стоимость)
    int prev; // номер предыдущей вершины в пути
};

class Graph {
    public:
        int numNodes; //количество вершин в графе
        int** matrix; //матрица смежности
        Node* nodes; //массив вершин (динамически выделяемый)
        int maxNodes; //максимальный размер графа

        //конструктор 
        Graph(int maxNodes,int numNodes) {
            this->nodes = new Node[maxNodes];
            this->maxNodes = maxNodes;
            this->numNodes = numNodes;
            matrix = new int*[maxNodes];
            for (int i = 0; i < maxNodes; i++) {
                matrix[i] = new int[maxNodes];
                for (int j = 0; j < maxNodes; j++) {
                    matrix[i][j] = INT_MAX; //расстояние по умолчанию
                }
            }
        }

        //деструктор 
        ~Graph() {
            for (int i = 0; i < numNodes; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;
        }

        //добавление ребра в граф
        void addEdge(int src, int dst, int weight, int time) {
            matrix[src][dst] = weight;
        }

        void addNode(const string& name) {
            //проверка на дубликаты
            for (int i = 0; i < numNodes; i++) {
                if (nodes[i].name == name) {
                    cout << "Ошибка: Город с названием " << name << " уже существует!" << endl;
                    return;
                }
            }

            //увеличение количества вершин
            if (numNodes == maxNodes) {
                cout << "Ошибка: Достигнут максимальный размер графа!" << endl;
                return;
            }
            
            //добавление новой вершины
            nodes[numNodes].id = numNodes;
            nodes[numNodes].name = name;
            numNodes++;
            
            //обновление матрицы смежности
            for (int i = 0; i < numNodes; i++) {
                matrix[i][numNodes - 1] = INT_MAX; //расстояние по умолчанию бесконечность
                matrix[numNodes - 1][i] = INT_MAX; 
            }
            
        }

        //поиск кратчайших путей (алгоритм дейкстры)
        vector<Node> dextra(int src) {
            vector<Node> nodes(numNodes);
            for (int i = 0; i < numNodes; i++) {
                nodes[i].id = i;
                nodes[i].distance = INT_MAX;
                nodes[i].prev = -1;
            }

            nodes[src].distance = 0;
            priority_queue<pair<int, int>> pq; //очередь приоритетов для хранения узлов
            pq.push({0, src}); //добавляем начальный узел

            while (!pq.empty()) {
                int dist = pq.top().first;
                int curr = pq.top().second;
                pq.pop();

                if (dist > nodes[curr].distance) {
                    continue;
                }

                for (int i = 0; i < numNodes; i++) {
                    if (matrix[curr][i] != INT_MAX && dist + matrix[curr][i] < nodes[i].distance) {
                        nodes[i].distance = dist + matrix[curr][i];
                        nodes[i].prev = curr;
                        pq.push({nodes[i].distance, i});
                    }
                }
            }
            return nodes;
        }

        //функция восстановления пути по предкам
        vector<string> reconstruct_path(vector<Node>& nodes, int src, int dst) {
            vector<string> path;
            if (nodes[dst].prev == -1) {
                return path; //путь не найден
            }

            int curr = dst;
            while (curr != src) {
                path.push_back(nodes[curr].name);
                curr = nodes[curr].prev;
            }
            path.push_back(nodes[src].name);
            reverse(path.begin(), path.end());
            return path;
        }
};

//поиск маршрута по минимальному времени/стоимости
vector<string> findPath(Graph& graph, string srcCity, string dstCity, bool minTime) {
    int src = -1, dst = -1;
    for (int i = 0; i < graph.numNodes; i++) {
        if (graph.nodes[i].name == srcCity) {
            src = i;
        }
        if (graph.nodes[i].name == dstCity) {
            dst = i;
        }
    }

    if (src == -1 || dst == -1) {
        cout << "Города не найдены в графе!" << endl;
        return {};
        
        vector<Node> nodes = graph.dextra(src);
        if (nodes[dst].distance == INT_MAX) {
            cout << "Путь между этими городами не существует!" << endl;
            return {};
        }

        vector<string> path = graph.reconstruct_path(nodes, src, dst);
        cout << "Маршрут: ";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << (i < path.size() - 1 ? " -> " : "");
        }
        cout << endl;

        if (minTime) {
            cout << "Общее время в пути: " << nodes[dst].distance << endl;
        }
        else {
            cout << "Общая стоимость перелета: " << nodes[dst].distance << endl;
        }
        return path;
    }
    vector <string> nul;
    return nul;
}

//функция поиска списка доступных городов по бюджету
vector<string> findCityBudget(Graph& graph, string srcCity, int budget, bool timeLimit) {
    int src = -1;
    for (int i = 0; i < graph.numNodes; i++) {
        if (graph.nodes[i].name == srcCity) {
            src = i;
        }
    }

    if (src == -1) {
        cout << "Город отправления не найден в графе!" << endl;
        return {};
    }

    vector<Node> nodes = graph.dextra(src);
    vector<string> normik_city;

    for (int i = 0; i < graph.numNodes; i++) {
        if (nodes[i].distance <= budget && nodes[i].distance != INT_MAX) {
            normik_city.push_back(graph.nodes[i].name);
        }
    }

    if (normik_city.empty()) {
        cout << "Ни одного города в пределах бюджета не найдено!" << endl;
    }
    else {
        cout << "Доступные города: ";
        for (int i = 0; i < normik_city.size(); i++) {
            cout << normik_city[i] << (i < normik_city.size() - 1 ? ", " : "");
        }
        cout << endl;
    }

    return normik_city;
}


vector<string> findCityTime(Graph& graph, string srcCity, int timeLimit) {
    int src = -1;
    for (int i = 0; i < graph.numNodes; i++) {
        if (graph.nodes[i].name == srcCity) {
            src = i;
        }
    }

    if (src == -1) {
        cout << "Город отправления не найден в графе!" << endl;
        return {};
    }

    queue<pair<int, int>> q; // Очередь для BFS
    q.push({src, 0}); // Добавляем начальный узел и время в пути (0)

    vector<bool> visited(graph.numNodes, false);
    visited[src] = true;

    vector<string> normik_city;

    while (!q.empty()) {
        int curr = q.front().first;
        int timeSpent = q.front().second;
        q.pop();

        if (timeSpent > timeLimit) {
            continue;
        }

        normik_city.push_back(graph.nodes[curr].name);

        for (int i = 0; i < graph.numNodes; i++) {
            if (graph.matrix[curr][i] != INT_MAX && !visited[i]) {
                visited[i] = true;
                q.push({i, timeSpent + graph.matrix[curr][i]});
            }
        }
    }

    if (normik_city.empty()) {
        cout << "Ни одного города в пределах временного лимита не найдено!" << endl;
    }
    else {
        cout << "Доступные города за " << timeLimit << " часов: ";
        for (int i = 0; i < normik_city.size(); i++) {
            cout << normik_city[i] << (i < normik_city.size() - 1 ? ", " : "");
        }
        cout << endl;
    }

    return normik_city;
}


//проверка возможности добраться до города с пересадками
bool peresadki(Graph& graph, string srcCity, string dstCity, int maxTransits) {
    int src = -1, dst = -1;
    for (int i = 0; i < graph.numNodes; i++) {
        if (graph.nodes[i].name == srcCity) {
            src = i;
        }

        if (graph.nodes[i].name == dstCity) {
            dst = i;
        }
    }

    if (src == -1 || dst == -1) {
        return false;
    }

    queue<pair<int, int>> q; //очередь для bfs
    q.push({src, 0}); //добавляем начальный узел и количество пересадок (0)

    vector<bool> visited(graph.numNodes, false);
    visited[src] = true;

    while (!q.empty()) {
        int curr = q.front().first;
        int transits = q.front().second;
        q.pop();

        if (curr == dst && transits <= maxTransits) {
            return true;
        }

        if (transits >= maxTransits) {
            continue;
        }

        for (int i = 0; i < graph.numNodes; i++) {
            if (graph.matrix[curr][i] != INT_MAX && !visited[i]) {
                visited[i] = true;
                q.push({i, transits + 1});
            }
        }
    }

    return false;
}

//поиск максимального расстояния, доступного с пересадками и бюджетом
int findMaxDist(Graph& graph, string srcCity, int budget) {
    int src = -1;
    for (int i = 0; i < graph.numNodes; i++) {
        if (graph.nodes[i].name == srcCity) {
            src = i;
        }
    }

    if (src == -1) {
        cout << "Город отправления не найден в графе!" << endl;
        return 0;
    }

    vector<Node> nodes = graph.dextra(src);
    int maxDistance = 0;

    for (int i = 0; i < graph.numNodes; i++) {
        if (nodes[i].distance <= budget && nodes[i].distance != INT_MAX) {
            maxDistance = max(maxDistance, nodes[i].distance);
        }
    }

    cout << "Максимальное доступное расстояние: " << maxDistance << endl;
    return maxDistance;
}

int main() {
    Graph graph(50,0);
    
    graph.addNode("Москва");
    graph.addNode("Санкт-Петербург");
    graph.addNode("Екатеринбург");
    graph.addNode("Краснодар");
    graph.addNode("Новосибирск");
    graph.addNode("Владивосток");
    
    //добавление ребер (стоимость/время перелета)
    graph.addEdge(0, 1, 1000, 2); //Москва - Санкт-Петербург (1000 рублей, 2 часа)
    graph.addEdge(0, 2, 2500, 4); //Москва - Екатеринбург (2500 рублей, 4 часа)
    graph.addEdge(0, 3, 3000, 3); //Москва - Краснодар (3000 рублей, 3 часа)
    graph.addEdge(1, 2, 1700, 3); //Санкт-Петербург - Екатеринбург (1700 рублей, 3 часа)
    graph.addEdge(1, 3, 2200, 4); //Санкт-Петербург - Краснодар (2200 рублей, 4 часа)
    graph.addEdge(2, 3, 1500, 2); //Екатеринбург - Краснодар (1500 рублей, 2 часа)
    graph.addEdge(2, 4, 2300, 5); //Екатеринбург - Новосибирск (2300 рублей, 5 часов)
    graph.addEdge(3, 4, 1800, 3); //Краснодар - Новосибирск (1800 рублей, 3 часа)
    graph.addEdge(4, 5, 3100, 6); //Новосибирск - Владивосток (3100 рублей, 6 часов)

    //поиск маршрута по минимальному времени
    string srcCity = "Москва";
    string dstCity = "Владивосток";
    
    vector<string> path = findPath(graph, srcCity, dstCity, true);
    cout << "Маршрут по минимальному времени (" << srcCity << " -> " << dstCity << "): ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i] << (i < path.size() - 1 ? " -> " : "");
    }
    cout << endl;

    //поиск доступных городов по бюджету
    int budget = 2000;
    vector<string> normik_city = findCityBudget(graph, srcCity, budget, true);
    cout << "Доступные города из " << srcCity << " с бюджетом " << budget << " рублей: ";
    for (int i = 0; i < normik_city.size(); i++) {
        cout << normik_city[i] << (i < normik_city.size() - 1 ? ", " : "");
    }
    cout << endl;

    return 0;
}
