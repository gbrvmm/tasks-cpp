/*
Класс контейнер для хранения записей с полями
• ФИО
• Дата рождения
• Объем мозга
Методы ввода, вывода, добавления данных из массива
Не меньше 10 методов сортировки (не больше 4х простых) с указанием поля, по которому сортировать
Внести не менее 100 записей.
Добавить мониторинг количества сравнений и количества перезаписей элементов
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include <fstream>
#include <sys/time.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
int random(int low, int high){
    uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

class Date {
    public:
        int day = 0, month = 0, year = 0;
        Date() {
            day = random(1, 31);
            month = random(1, 12);
            year = random(1950, 2023);
        };

        friend ostream& operator << (ostream& stream, const Date& p){
            stream << p.day << "." << p.month << "." << p.year;
            return stream;
        }
};

class People {
    public:
        string name, surname, secondname;
        Date dr;
        int brain;

        People() {
            brain = random(1,1000);
            name = creat_str();
            surname = creat_str();
            secondname = creat_str();
        }

        string creat_str(){
            string abc = "qazwsxedcrfvtgbyhnujmikolp";
            string new_str = "";
            for (int i = 0; i < 5; i++){
                new_str += abc[random(0,25)];
            }
            return new_str;
        }

        friend ostream& operator << (ostream& stream, const People& p){
            stream << p.name << " ^_^ " << p.surname << " ^_^ " << p.secondname << " ^_^ " << p.dr  << " ^_^ " << p.brain << endl;
            return stream;
        }
};


class Container {
    public:
        vector <People> main_array;
        Container(){
            for(int i = 0; i < 100; i++) {
                People people;
                main_array.emplace_back(people);
            }
        }

        void get_record(int i){
            cout << main_array[i];
        }

        //пузырьковая сортировка (Bubble sort) по имени
        void bubble_sort_by_name() {
            int iter = 0, swaps = 0;
            int n = main_array.size();
            iter++;
            for (int i = 0; i < n - 1; ++i) {
                iter++;
                for (int j = 0; j < n - i - 1; ++j) {
                    if (main_array[j].name > main_array[j + 1].name) {
                        swap(main_array[j], main_array[j + 1]);
                        swaps++;
                    }
                }
            }
            cout << "Сортировка пузырьком   " << endl << "итерации: " << iter << "   " << "перезаписи: " << swaps << endl;

        }

        //cортировка выбором (Selection sort) по фамилии
        void selection_sort_by_surname() {
            int iter = 0, swaps = 0;
            int n = main_array.size();
            for (int i = 0; i < n - 1; ++i) {
                iter++;
                int min_idx = i;
                for (int j = i + 1; j < n; ++j) {
                    iter++;
                    if (main_array[j].surname < main_array[min_idx].surname) {
                        min_idx = j;
                        swaps++;
                    }
                }
                swap(main_array[i], main_array[min_idx]);
            }
            cout << "Сортировка выбором  " << endl << "итерации: " << iter << "   " << "перезаписи: " << swaps << endl;

        }

        //сортировка вставками (Insertion sort) по отчеству
        void insertion_sort_by_secondname() {
            int iter = 0, swaps = 0;
            int n = main_array.size();
            for (int i = 1; i < n; ++i) {
                iter++;
                People key = main_array[i];
                int j = i - 1;
                while (j >= 0 && main_array[j].secondname > key.secondname) {
                    main_array[j + 1] = main_array[j];
                    --j;
                    swaps++;
                }
                main_array[j + 1] = key;
            }
            cout << "Сортировка вставками  " << endl << "итерации: " << iter << "   " << "перезаписи: " << swaps << endl;
        }

        //сортировка слиянием (Merge sort) по объему мозга
        int iter_merge = 0; int swaps_merge = 0;
        void merge_sorting() {
            merge_sort_by_brain(0, main_array.size());
            cout << "Cортировка слиянием " << endl << "итерации: " << iter_merge << "   " << "перестановки: " << swaps_merge << endl;
        }

        void merge_sort_by_brain(int low, int high) {
            if (low < high) {
                int mid = low + (high - low) / 2;
                merge_sort_by_brain(low, mid);
                merge_sort_by_brain(mid + 1, high);
                merge_by_brain(low, mid, high);
            }
        }

        void merge_by_brain(int low, int mid, int high) {
            int n1 = mid - low + 1;
            int n2 = high - mid;

            vector<People> L(n1), R(n2);
            for (int i = 0; i < n1; ++i) {
                iter_merge++;
                L[i] = main_array[low + i];
            }
            for (int j = 0; j < n2; ++j) {
                iter_merge++;
                R[j] = main_array[mid + 1 + j];
            }

            int i = 0, j = 0, k = low;
            while (i < n1 && j < n2) {
                iter_merge++;
                if (L[i].brain <= R[j].brain) {
                    main_array[k] = L[i];
                    ++i;
                    iter_merge++;
                } else {
                    main_array[k] = R[j];
                    ++j;
                    swaps_merge++;
                }
                ++k;
            }

            while (i < n1) {
                iter_merge++;
                main_array[k] = L[i];
                ++i;
                ++k;
                swaps_merge++;
            }

            while (j < n2) {
                iter_merge++;
                main_array[k] = R[j];
                ++j;
                ++k;
                swaps_merge++;
            }
        }

        //сортировка шелла (Shell sort) по имени
        void shell_sort_by_name() {
            int iter = 0, swaps = 0;
            int n = main_array.size();
            for (int gap = n / 2; gap > 0; gap /= 2) {
                iter++;
                for (int i = gap; i < n; ++i) {
                    iter++;
                    People temp = main_array[i];
                    int j;
                    for (j = i; j >= gap && main_array[j - gap].name > temp.name; j -= gap) {
                        main_array[j] = main_array[j - gap];
                        swaps++;
                    }
                    main_array[j] = temp;
                }
            }
            cout << "Сортировка Шелла  " << endl << "итерации: " << iter << "   " << "перестановки: " << swaps << endl;
        }

        //сортировка кучей (Heap sort) по фамилии
        int iter_heap = 0, swaps_heap = 0;
        void heap_sort_by_surname() {
            int n = main_array.size();
            for (int i = n / 2 - 1; i >= 0; --i) {
                iter_heap++;
                heapify_by_surname(n, i);
            }
            for (int i = n - 1; i > 0; --i) {
                iter_heap++;
                swap(main_array[0], main_array[i]);
                swaps_heap++;
                heapify_by_surname(i, 0);
            }
            cout << "Сортировка кучей  " << endl << "итерации: " << iter_heap << "   " << "перестановки: " << swaps_heap << endl;
        }

        void heapify_by_surname(int n, int i) {
            int largest = i;
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            if (l < n && main_array[l].surname > main_array[largest].surname)
                largest = l;
            if (r < n && main_array[r].surname > main_array[largest].surname)
                largest = r;
            if (largest != i) {
                swap(main_array[i], main_array[largest]);
                swaps_heap++;
                heapify_by_surname(n, largest);
            }
        }

        // быстрая сортировка по объему мозга
        int iter_quick = 0; int swaps_quick = 0;
        void quick_sort_by_brain() {
            int low = 0;
            int high = main_array.size() - 1;
            vector<int> stack(high - low + 1);
            int top = -1;
            stack[++top] = low;
            stack[++top] = high;
            while (top >= 0) {
                high = stack[top--];
                low = stack[top--];
                int pi = partition_by_brain(low, high);
                if (pi - 1 > low) {
                    stack[++top] = low;
                    stack[++top] = pi - 1;
                }
                if (pi + 1 < high) {
                    stack[++top] = pi + 1;
                    stack[++top] = high;
                }
            }
        }

        int partition_by_brain(int low, int high) {
            int pivot = main_array[high].brain;
            int i = low - 1;
            for (int j = low; j < high; ++j) {
                iter_quick++;
                if (main_array[j].brain < pivot) {
                    ++i;
                    swap(main_array[i], main_array[j]);
                    swaps_quick++;
                }
            }
            swap(main_array[i + 1], main_array[high]);
            return i + 1;
        }

        void quick_sort(){
            partition_by_brain(0, main_array.size());
            cout << "Быстрая сортировка  " << endl << "итерации: " << iter_quick << "   " << "перестановки: " << swaps_quick << endl;
        }


        //гномья сортировка
        void gnome_sort_by_name() {
            int iter = 0, swaps = 0;
            int index = 0;
            while (index < main_array.size()) {
                iter++;
                if (index == 0)
                    index++;
                if (main_array[index].name >= main_array[index - 1].name)
                    index++;
                else {
                    swap(main_array[index], main_array[index - 1]);
                    swaps++;
                    index--;
                }
            }
            cout << "Гномья сортировка  " << endl << "итерации: " << iter << "   " << "перестановки: " << swaps << endl;
        }

        //сортировка расческой
        void comb_sort_by_name() {
            int iter = 0, swaps = 0;
            const double factor = 1.247; //фактор уменьшения
            double step = main_array.size() - 1;
            while (step >= 1) {
                iter++;
                for (int i = 0; i + step < main_array.size(); ++i) {
                    iter++;
                    if (main_array[i].name > main_array[i + step].name) {
                        swaps++;
                        swap(main_array[i], main_array[i + step]);
                    }
                }
                step /= factor;
            }
            cout << "Сортировка расческой " << endl << "итерации: " << iter << "   " << "перестановки: " << swaps << endl;
        }

        //пирамидальная сортировка
        int iter_piramida = 0, swaps_piramida = 0;
        void heapify(unsigned int n, unsigned int i) {
            unsigned int largest = i;
            unsigned int left = 2 * i + 1;
            unsigned int right = 2 * i + 2;
            if (left < n && main_array[left].name > main_array[largest].name) {
                largest = left;
            }
            if (right < n && main_array[right].name > main_array[largest].name) {
                largest = right;
            }
            if (largest != i) {
                swap(main_array[i], main_array[largest]);
                swaps_piramida++;
                heapify(n, largest);
            }
        }

        void piramida_sort_by_name() {
            for (int i = main_array.size() / 2 - 1; i >= 0; i--) {
                iter_piramida++;
                heapify(main_array.size(), i);
            }
            for (int i = main_array.size() - 1; i > 0; i--) {
                iter_piramida++;
                if (main_array[0].name > main_array[i].name) {
                    swap(main_array[0], main_array[i]);
                    heapify(i, 0);
                }
            }
            cout << "Пирамидальная сортировка  " << endl << "итерации: " << iter_piramida << "   " << "перестановки: " << swaps_piramida << endl;
        }

        void output() {
            for (int i = 0; i < 100; i++) {
                cout << i + 1 << ". ";
                get_record(i);
            }
        }

};

int main(){
    Container cont;

    struct timeval start, end;
    ios_base::sync_with_stdio(false);
    double time_taken;

    gettimeofday(&start, NULL);
    cont.comb_sort_by_name();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << fixed;
    cout.precision(6);
    cout << time_taken << "sec" << endl << endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.bubble_sort_by_name();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl << endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.selection_sort_by_surname();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.insertion_sort_by_secondname();;
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.merge_sorting();
    gettimeofday(&end, NULL);
    cout << cont.iter_merge << cont.swaps_merge;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.shell_sort_by_name();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.heap_sort_by_surname();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.quick_sort_by_brain();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.gnome_sort_by_name();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    gettimeofday(&start, NULL);
    cont.piramida_sort_by_name();
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << time_taken << "sec" << endl<< endl;
    cont.output();

    return 0;
}
