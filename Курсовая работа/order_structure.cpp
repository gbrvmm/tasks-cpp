#include <iostream>
#include <string>

using namespace std;

// информация про клиента
class clients {
    private:
        string name, surname, number;
    public:
        string get_name() {
            return name;
        }
        string get_surname() {
            return surname;
        }
        string get_number() {
            return number;
        }

        clients() {
            name = "Алексей";
            surname = "Ковригин";
            number = "81234567890";
        }

        clients(string name, string surname, string number) : name(name), surname(surname), number(number) {}

        void output() {
            cout << surname << " " << name << ", телефон для связи: " << number << endl;
        }
};

// cтруктура заказа
class order_structure {
    private:
        string gruppa, usluga;
        int grade;
        clients client;
        string students[5] = {
            "Студенты ФИ","Студенты БИ","Студенты ИН",
            "Студенты ИСИТ","Студенты СТ"
        };
        string uslugi[5] = {
            "Знать С++","Знать Python",
            "Знать CSS","Знать HTML", "Знать JavaScript"
        };
        
    public:
        order_structure() {
            gruppa = "Cтуденты ФИ";
            grade = 5;
            usluga = "Знать С++";
        }
        order_structure(string name, string surname, string number, string gruppa, int grade, string usluga) : 
        client(*(new clients(name, surname, number))), grade(grade), gruppa(gruppa), usluga(usluga) {}
        
        string get_gruppa() {
            return gruppa;
        }

        int get_grade() {
            return grade;
        }

        string get_usluga() {
            return usluga;
        }
        
        clients get_client() {
            return client;
        }

        void output() {
            client.output();
            cout << gruppa << " должны на оценку " << grade << " " << usluga << endl;
        }

        void redact() {
            cout << "Выберите пункт заказа, который необходимо отредактировать:\n";
            cout << "1. Группа студентов \n";
            cout << "2. Оценка \n";
            cout << "3. Услуга \n";
            cout << "4. Информация о клиенте \n";
            int vibor = 0;
            while (vibor < 1 || vibor > 4){
                cin >> vibor;
            }
            switch (vibor) {

                case 1: {
                    cout << "Выберите группу студентов:" << endl;
                    int vibor = 0;
                    for (int i = 0; i < 5; i++) {
                        cout << i + 1 << ". " << students[i] << endl;
                    }
                    while (vibor < 1 || vibor > 6) {
                        cin >> vibor;
                    }
                    gruppa = students[vibor - 1];
                    break;
                }

                case 2: {
                    cout << "Введите оценку, на которую должна быть оказана услуга (1-5): ";
                    cin >> grade;
                    break;
                }

                case 3: {
                    int vibor = 0;
                    cout << "Выберите услугу: " << endl;
                    for (int i = 0; i < 5; i++) {
                        cout << i + 1 << ". " << uslugi[i] << endl;
                    }
                    while (vibor < 1 || vibor > 5) {
                        cin >> vibor;
                    }
                    usluga = uslugi[vibor - 1];
                }

                case 4: {
                    string name, surname, number;
                    cout << "Имя: ";
                    cin >> name;
                    cout << "Фамилия: ";
                    cin >> surname;
                    cout << "Номер телефона для связи: ";
                    cin >> number;
                    client = clients(name, surname, number);
                    break;
                }
            }
        }
};