/*
Задание 6

Прямоугольное поле шириной N и длиной M метров разбит на квадраты равной величины. На этом участке разбиты участки.
Участком называется совокупность квадратов, удовлетворяющая таким условиям:
- из любого квадрата участка можно попасть в любой другой квадрат участка, последовательно переходя по участку из
квадрата в квадрат через их общую сторону;
- никакие два участка не пересекаются и не касаются друг друга ни по вертикальной, ни по горизонтальной сторонам
квадратов (касание участков углами квадратов допускается).
Подсчитайте количество участков на поле.
*/

#include <iostream>
#include <fstream>
using namespace std;

class Field {
    private:
        int N, M;
        char** field; //инфа о символах на поле
        bool** visited; //инфа о посещении

    public:
        Field(int n, int m) {
            N = n;
            M = m;
            field = new char*[N];
            visited = new bool*[N];

            for (int i = 0; i < N; i++) {
                field[i] = new char[M];
                visited[i] = new bool[M];
            }
        }

        //считывание из файла
        void read(ifstream& fin) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    fin >> field[i][j];
                    visited[i][j] = false;
                }
            }
        }

        //является ли клетка "#"
        bool isValid(int x, int y) {
            return (x >= 0 && x < N && y >= 0 && y < M && !visited[x][y] && field[x][y] == '#');
        }

        //обход в глубину
        void dfs(int x, int y) {
            int new_x[4] = {1, -1, 0, 0};
            int new_y[4] = {0, 0, 1, -1};

            visited[x][y] = true;

            for (int i = 0; i < 4; i++) {
                int newX = x + new_x[i];
                int newY = y + new_y[i];
                if (isValid(newX, newY)) {
                    dfs(newX, newY);
                }
            }
        }

        int countRegions() {
            int count = 0;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    if (!visited[i][j] && field[i][j] == '#') {
                        dfs(i, j);
                        count++;
                    }
                }
            }
            return count;
        }
};

int main() {
    int N, M;
    ifstream fin("input.txt");
    fin >> N >> M;
    Field field(N, M);
    field.read(fin);
    fin.close();
    int regions = field.countRegions();
    cout << regions << endl;
    ofstream fout("output.txt");
    fout << regions << endl;
    fout.close();

    return 0;
}