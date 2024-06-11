#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "order_structure.cpp"

using namespace std;

// все заказы и работа с ними
class production_orders {
    private:
        int n; // количество заказов
        vector <order_structure> orders; // список заказов 

        string students[5] = {
            "Студенты ФИ","Студенты БИ","Студенты ИН",
            "Студенты ИСИТ","Студенты СТ"
        };

        string uslugi[5] = {
            "Знать С++","Знать Python",
            "Знать CSS","Знать HTML", "Знать JavaScript"
        };

    public: 
        production_orders() {
            n = 1;
            order_structure defolt_order = order_structure( "Алексей", "Ковригин", "8-999-999-99-99", "Студенты ФИ", 5, "знать С++");
            orders.emplace_back(defolt_order);
        }

        int get_n() {
            return n;
        }
        
        // вывод всех заказов
        void output() {
            for (int i = 0; i < n; i++) {
                order_structure current_order = orders[i];
                cout << i + 1 << ". ";
                current_order.output();
            }
        }

        // добавить заказ
        void add () {
            string gruppa, usluga, name, surname, number;
            int grade;
            cout << "Выберите группу студентов:" << endl;
            int vibor = 0;
            for (int i = 0; i < 5; i++) {
                cout << i + 1 << ". " << students[i] << endl;
            }
            while (vibor < 1 || vibor > 5) {
                cin >> vibor;
            }
            gruppa = students[vibor - 1];

            cout << "Укажите оценку, на которую должна быть выполнена услуга (1-5): ";
            cin >> grade;
            vibor = 0;

            cout << "Выберите услугу:" << endl;
            for (int i = 0; i < 5; i++) {
                cout << i + 1 << ". " << uslugi[i] << endl;
            }
            while (vibor < 1 || vibor > 5) {
                cin >> vibor;
            }
            usluga = uslugi[vibor - 1];

            cout << "Имя: ";
            cin >> name;

            cout << "Фамилия: ";
            cin >> surname;

            cout << "Номер телефона для связи: ";
            cin >> number;

            order_structure new_order(name, surname, number, gruppa, grade, usluga); // создание 1 заказа
            orders.emplace_back(new_order); // добавление в конец списка заказов
            n++;
        }

        //удалить заказ
        void delete_order(int k) {
            orders.erase(orders.begin() + k - 1); 
            n--;
        }

        //редактировать заказ
        void redact_order(int n) {
            orders[n - 1].redact();
        }

        //загрузить в файл
        void add_to_file() { 
            ofstream output_file;
            output_file.open("orders_not_stupid_students.txt", ios::app);
            if (output_file.is_open()) {
                string order_information;
                for (int i = 0; i < n; i++) {
                    order_structure current_order = orders[i];
                    order_information = current_order.get_client().get_name() + "^_^" + current_order.get_client().get_surname() + "^_^" + current_order.get_client().get_number() + "^_^" + current_order.get_gruppa() + "^_^" + to_string(current_order.get_grade()) + "^_^" + current_order.get_usluga();
                    output_file << order_information << endl;
                }
                output_file.close();
            }
        }

        // загрузить заказы из файла
        void add_from_file() {
            ifstream input_file("orders_not_stupid_students.txt");
            if (input_file.is_open()){
                string new_string;
                while (getline(input_file, new_string)) {
                    string name = new_string.substr(0, new_string.find("^_^"));
                    new_string = new_string.substr(new_string.find("^_^") + 3, new_string.length() - new_string.find("^_^"));

                    string surname = new_string.substr(0, new_string.find("^_^"));
                    new_string = new_string.substr(new_string.find("^_^") + 3, new_string.length() - new_string.find("^_^"));

                    string number = new_string.substr(0, new_string.find("^_^"));
                    new_string = new_string.substr(new_string.find("^_^") + 3, new_string.length() - new_string.find("^_^"));

                    string gruppa = new_string.substr(0, new_string.find("^_^"));
                    new_string = new_string.substr(new_string.find("^_^") + 3, new_string.length() - new_string.find("^_^"));

                    int grade = stof(new_string.substr(0, new_string.find("^_^")));
                    new_string = new_string.substr(new_string.find("^_^") + 3, new_string.length() - new_string.find("^_^"));

                    string usluga = new_string.substr(0, new_string.length());
                    order_structure new_order(name, surname, number, gruppa, grade, usluga);
                    orders.emplace_back(new_order);
                    n++;
                }
            }
            input_file.close();
        }
};