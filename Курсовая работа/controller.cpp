#include <iostream>
#include "production_orders.cpp"

using namespace std;

class controller {
    private:
        production_orders current_order;
        
    public:
        controller() {}
        void execute() {
            int run = 1;
            while (run) {
                menu();
                int var = 0;
                while (var < 1 || var > 7) {
                    cout << "Введите номер команды: ";
                    cin >> var;
                }

                if (var == 7) {
                    run = 0;
                }

                if (var == 1) {
                    current_order.output();
                }

                if (var == 2) {
                    current_order.add ();
                    cout << "Заказ создан ^_^ \n";
                }

                if (var == 3) {
                    current_order.output();
                    cout << "Введите номер заказа, который необходимо удалить:\n";
                    int n;
                    cin >> n;
                    current_order.delete_order(n);
                    cout << "Заказ удален ^_^ \n";
                }

                if (var == 4) {
                    cout << "Выберите заказ, который необходимо отредактировать: \n";
                    int vibor = 0;
                    while (vibor < 1 || vibor > current_order.get_n() + 1) {
                        current_order.output();
                        cout << current_order.get_n() + 1 << ". " << "Отмена редактирования!!! \n";
                        cin >> vibor;
                    }
                    current_order.redact_order(vibor);
                    cout << "Заказ отредактирован ^_^ \n";
                }

                if (var == 5) {
                    current_order.add_to_file();
                    cout << "Заказы сохранены в файл ^_^ \n";
                }

                if (var == 6) {
                    current_order.add_from_file();
                    cout << "Заказы из файла загружены ^_^ \n";
                }

            }
        }

        void menu() {
            cout << "Команды: " << endl;
            cout << "1. Вывести список заказов" << endl;
            cout << "2. Создать новый заказ" << endl;
            cout << "3. Удалить заказ" << endl;
            cout << "4. Редактирование заказа" << endl;
            cout << "5. Добавить заказы в файл" << endl;
            cout << "6. Загрузить заказы из файла" << endl;
            cout << "7. Выход из программы" << endl;
        }
};

