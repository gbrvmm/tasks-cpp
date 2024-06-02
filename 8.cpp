/*
Задание 8

Реализовать класс multiplTree, реализующий множественное дерево хранения данных.
Каждый узел имеет текстовое название  и может хранить тестовые данные и указатели на неограниченное число потомков. 
Для узла дерева реализовать следующие методы:
setName - установка имени узла.
getName - получение имени узла.
setValue (перегрузить данный метод для установки текстового значения, значения double, int, данные сохраняются в виде текстового значения), 
getValue - получение текстового значения узла.
isInt - проверяет можно ли представить данные в виде целого числа. 
isDouble - проверяет можно ли представить данные узла в виде действительного числа. 
getInt - получение данных узла в виде целого числа. При невозможности выдает 0. 
getDouble - получение данных узла  действительного числа. При невозможности выдает 0.
getCountSub - выдает количество потомков.
getSub(i) - выдает потомка по порядковому номеру. При невозможности выдает Null.
getSub(name) - выдает потомка по порядковому имени. При невозможности выдает Null.
addSub - добавляет узел потомок.
Для всего класса реализовать поиск узла (первое вхождение) по заданному  имени. 

С помощью класса multiplTree реализовать работу с данными в формате XML - загрузка из текстового формата и выдачу в удобном для
обработки порядке. Добавить приколов, сделать консольный редактор
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

class Node {
    private:
        string name;
        string value;
        vector<Node> children;

    public:
        Node() : name(""), value("") {}

        void setName(const string& name) {
            this->name = name;
        }

        string getName() const {
            return name;
        }

        void setValue(const string& value) {
            this->value = value;
        }

        void setValue(const double& value) {
            this->value = to_string(value);
        }

        void setValue(const int& value) {
            this->value = to_string(value);
        }

        string getValue() const {
            return value;
        }

        bool isInt() const {
            try {
                stoi(value);
                return true;
            } catch (...) {
                return false;
            }
        }

        bool isDouble() const {
            try {
                stod(value);
                return true;
            } catch (...) {
                return false;
            }
        }

        int getInt() const {
            if (isInt())
                return stoi(value);
            else
                return 0;
        }

        double getDouble() const {
            if (isDouble())
                return stod(value);
            else
                return 0.0;
        }

        int getCountSub() const {
            return children.size();
        }

        Node* getSub(int i) {
            if (i >= 0 && i < children.size())
                return &children[i];
            else
                return nullptr;
        }

        Node* getSub(const string& name) {
            for (int i = 0; i < children.size(); ++i) {
                if (children[i].getName() == name)
                    return &children[i];
            }
            return nullptr;
        }

        void addSub(Node node) {
            children.push_back(node);
        }

        void removeSub(int index) {
            if (index >= 0 && index < children.size()) {
                children.erase(children.begin() + index);
            }
        }

        void print(ostream& out = cout, int indent = 0) const {
            string indentation(indent, ' ');
            out << indentation << '<' << name << '>';
            if (!value.empty()) {
                out << value;
            }
            for (const auto& child : children) {
                out << endl;
                child.print(out, indent + 2);
            }
            if (!children.empty()) {
                out << endl << indentation;
            }
            out << "</" << name << '>';
        }

};

class multiplTree {
    private:
        Node root;

    public:
        void loadFromFile(const string& filename) {
            ifstream file(filename);
            if (!file.is_open()) {
                cerr << "Ошибка: не удалось открыть файл " << filename << endl;
                return;
            }

            stack<Node*> nodeStack;
            nodeStack.push(&root);

            string line;
            while (getline(file, line)) {
                if (line.empty()) {
                    continue;
                }

                line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
                line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

                Node* currentNode = nodeStack.top();


                if (line[0] == '<' && line[1] != '/') {
                    size_t endPos = line.find('>');
                    string tagName = line.substr(1, endPos - 1);

                    Node newNode;
                    newNode.setName(tagName);
                    currentNode->addSub(newNode);
                    if (root.getName() == ""){root = newNode;}else {
                        nodeStack.push(currentNode->getSub(currentNode->getCountSub() - 1));
                    }
                }
                else if (line[0] == '<' && line[1] == '/') {
                    nodeStack.pop();
                }
                else {
                    currentNode->setValue(line);
                }
            }
            file.close();
        }


        void saveToFile(const string& filename) const {
            ofstream file(filename);
            if (!file.is_open()) {
                cerr << "Ошибка: не удалось открыть файл " << filename << endl;
                return;
            }
            root.print(file);
            file.close();
        }

        Node* getRoot() {
            return &root;
        }

        void print() const {
            root.print();
        }
};

void print_menu() {
    cout << "Меню:" << endl;
    cout << "1. Вывести XML файл" << endl;
    cout << "2. Изменить имя узла" << endl;
    cout << "3. Изменить значение узла" << endl;
    cout << "4. Удалить дочерний узел из выбранного узла" << endl;
    cout << "5. Сохранить файл" << endl;
    cout << "0. Выход" << endl;
    cout << "Введите номер команды: ";
}

Node* selectNode(Node* root) {
    Node* currentNode = root;
    string nodeName;

    while (true) {
        cout << "Текущий узел: " << currentNode->getName() << " (" << currentNode->getValue() << ")" << endl;
        cout << "Введите имя подузла (или «Готово», чтобы выбрать этот узел): ";
        cin >> nodeName;

        if (nodeName == "Готово") {
            return currentNode;
        }

        Node* nextNode = currentNode->getSub(nodeName);
        if (nextNode) {
            currentNode = nextNode;
        } else {
            cout << "Узел не найден" << endl;
        }
    }
}

void changeNodeName(Node* node) {
    string newName;
    cout << "Введите новое имя: ";
    cin.ignore();  
    getline(cin, newName);
    node->setName(newName);
}

void changeNodeValue(Node* node) {
    string newValue;
    cout << "Введите новое значение: ";
    cin.ignore();
    getline(cin, newValue);
    node->setValue(newValue);
}

void deleteNode(Node* parent, int index) {
    if (parent) {
        parent->removeSub(index);
    }
}

int main() {
    multiplTree tree;
    tree.loadFromFile("books.xml");

    while (true) {
        print_menu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                tree.print();
                cout << endl;
                break;
            case 2: {
                Node* node = selectNode(tree.getRoot());
                if (node) {
                    changeNodeName(node);
                }
                break;
            }
            case 3: {
                Node* node = selectNode(tree.getRoot());
                if (node) {
                    changeNodeValue(node);
                }
                break;
            }
            case 4: {
                Node* parentNode = selectNode(tree.getRoot());
                if (parentNode) {
                    cout << "Введите индекс подузла для удаления: ";
                    int index;
                    cin >> index;
                    deleteNode(parentNode, index);
                }
                break;
            }
            case 5:
                tree.saveToFile("books.xml");
                break;
            case 0:
                return 0;
            default:
                cout << "Неправильный выбор" << endl;
        }
    }

    return 0;
}