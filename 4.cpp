/*
Задание 4
Реализовать оберточный класс Graph, содержащий вершины графа и ребра между ними в виде элементов матрицы смежности.
Реализовать алгоритм Дейкстры. При помощи этого алгоритма реализовать решение следующих задач:
На вход подается список городов, расстояния между ними, время и стоимость перелета между парами городов разными рейсами
разных авиакомпаний. При помощи алгоритмов поиска минимального остовного дерева реализовать поиск минимального набора
рейсов, в условиях санкций и нехватки самолетов.
*/

#include <iostream>
#include <climits>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Graph {
    private:
        int vertices; //количество вершин
        int** matrix; //матрица смежности

    public:
        Graph(int vertices) {
            this->vertices = vertices;
            matrix = new int*[vertices];
            for (int i = 0; i < vertices; i++) {
                matrix[i] = new int[vertices];
                for (int j = 0; j < vertices; j++) {
                    matrix[i][j] = INT_MAX; //расстояние по умолчанию бесконечность
                }
            }
        }

        ~Graph() {
            for (int i = 0; i < vertices; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;
        }

        void addEdge(int source, int destination, int weight) {
            if (source < 0 || source >= vertices || destination < 0 || destination >= vertices) {
                return;
            }
            matrix[source][destination] = weight;
        }

        void printGraph() {
            for (int i = 0; i < vertices; i++) {
                for (int j = 0; j < vertices; j++) {
                    cout << matrix[i][j] << " ";
                }
                cout << endl;
            }
        }

        int getVertices() const {
            return vertices;
        }

        void dextra_algorithm(int source) {
            int* distances = new int[vertices]; //массив расстояний
            bool* visited = new bool[vertices]; //массив посещенных вершин

            //инициализация массивов
            for (int i = 0; i < vertices; i++) {
                distances[i] = INT_MAX;
                visited[i] = false;
            }

            distances[source] = 0; //расстояние до начальной вершины = 0

            for (int i = 0; i < vertices; i++) {
                //выбираем необработанную вершину с минимальным расстоянием
                int min_distance = INT_MAX;
                int min_distance_index = -1;
                for (int j = 0; j < vertices; j++) {
                    if (!visited[j] && distances[j] < min_distance) {
                        min_distance = distances[j];
                        min_distance_index = j;
                    }
                }

                //если все вершины обработаны, то конец (света)
                if (min_distance_index == -1) {
                    break;
                }

                visited[min_distance_index] = true; //отмечаем вершину как посещенную

                //обновляем расстояния до соседних вершин
                for (int j = 0; j < vertices; j++) {
                    if (matrix[min_distance_index][j] != INT_MAX && !visited[j]) {
                        int new_distance = distances[min_distance_index] + matrix[min_distance_index][j];
                        if (new_distance < distances[j]) {
                            distances[j] = new_distance;
                        }
                    }
                }
            }

            //вывод расстояний до всех вершин
            for (int i = 0; i < vertices; i++) {
                cout << "Расстояние от " << source << " до " << i << ": " << distances[i] << endl;
            }

            delete[] distances;
            delete[] visited;
        }
};

//класс с информацией о рейсе
class Flight {
    public:
        int source; //город отправления
        int destination; //город назначения
        int airline; //авиакомпания
        int time; //время в пути
        int cost; //стоимость
};

//сравнение рейсов по времени
bool flights_time(const Flight& flight1, const Flight& flight2) {
    return flight1.time < flight2.time;
}

//сравнение рейсов по стоимости
bool flights_cost(const Flight& flight1, const Flight& flight2) {
    return flight1.cost;
}

//функция поиска минимального остовного дерева
vector<Flight> findMinTree(Graph& graph, map<pair<int, int>, vector<Flight> >& flights) {
    vector<Flight> mst; //минимальное остовное дерево

    //инициализация вспомогательных структур
    set<pair<int, int> > edges; //множество ребер
    map<int, int> parent; //массив родителей


    //добавление всех ребер в множество
    for (int i = 0; i < graph.getVertices(); i++) {
        for (int j = i + 1; j < graph.getVertices(); j++) {
            if (flights.count({i, j}) > 0) {
                edges.insert({i, j});
            }
        }
    }

    //алгоритм прима
    while (!edges.empty()) {
        //находим ребро с минимальным весом
        auto minEdge = *edges.begin();
        edges.erase(minEdge);

        int source = minEdge.first;
        int destination = minEdge.second;

        //добавляяем ребро в дерево
        mst.push_back(flights[{source, destination}][0]); //выбираем первый рейс

        //обновляем информацию о родителях
        if (parent.count(destination) == 0) {
            parent[destination] = source;
        }

        if (edges.size() > 0){continue;}

        //оставляяем только те ребра, которые не образуют цикл
        for (const pair<int, int> &edge: edges) {
            // pair<int, int> edge = edges[];
            int newSource = edge.first;
            int newDestination = edge.second;

            if (parent.count(newDestination) > 0 && parent[newDestination] == source) {
                edges.erase(edge);
            }
        }
    }
    return mst;
}

int main() {
    int vertices = 5; //количество городов
    Graph graph(vertices);

    //добавление ребер (городов)
    graph.addEdge(0, 1, 50);
    graph.addEdge(0, 2, 70);
    graph.addEdge(1, 2, 40);
    graph.addEdge(1, 3, 60);
    graph.addEdge(2, 3, 30);
    graph.addEdge(2, 4, 80);
    graph.addEdge(3, 4, 20);

    //данные о рейсах
    map<pair<int, int>, vector<Flight>> flights;
    flights[{0, 1}] = {{0, 1, 1, 60, 200}, {0, 1, 2, 50, 300}};
    flights[{0, 2}] = {{0, 2, 1, 70, 150}, {0, 2, 2, 60, 250}};
    flights[{1, 2}] = {{1, 2, 1, 40, 100}, {1, 2, 2, 30, 200}};
    flights[{1, 3}] = {{1, 3, 1, 60, 180}, {1, 3, 2, 50, 270}};
    flights[{2, 3}] = {{2, 3, 1, 30, 90}, {2, 3, 2, 20, 180}};
    flights[{2, 4}] = {{2, 4, 1, 80, 140}, {2, 4, 2, 70, 230}};
    flights[{3, 4}] = {{3, 4, 1, 20, 50}, {3, 4, 2, 10, 100}};

    //поиск минимального остовного дерева
    vector<Flight> mst = findMinTree(graph, flights);

    //вывод минимального набора рейсов
    cout << "Минимальный набор рейсов:" << endl;
    for (const Flight& flight : mst) {
        cout << "Из " << flight.source << " в " << flight.destination << " рейсом " << flight.airline << ": время - " << flight.time << ", стоимость - " << flight.cost << endl;
    }
    return 0;
}


