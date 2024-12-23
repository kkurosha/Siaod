#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <stack>
using namespace std;

enum Color { RED, BLACK }; //цвет узла

struct Node { //структура узла
    string name; 
    bool color;;
    Node* left;  //указатель на левое поддерево
    Node* right; //указатель на правое поддерево
    Node* parent; //указатель на родителя
    Node(string name) : name(name), color(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root; //корень дерева

    void rotateRight(Node*& node) { //поворот вправо
        Node* newParent = node->left; //указатель указывает на левый узел текущего узла
        node->left = newParent->right; //перенаправление левого ребенка на правого
        if (newParent->right != nullptr) //если правый ребненок есть
            newParent->right->parent = node; //его родитель - текущий узел
        newParent->parent = node->parent;
        if (node->parent == nullptr) root = newParent; //если node-корень, обновляем указатель на корень
        else if (node == node->parent->right) //если node-правый ребенок
            node->parent->right = newParent; //обновляем указатель родителя
        else node->parent->left = newParent; //если левый ребенок, обновляем указатель
        newParent->right = node;
        node->parent = newParent;
    }

    void rotateLeft(Node*& node) { //поворот налево
        Node* newParent = node->right; //указатель на правый дочерний узел текущего узла
        node->right = newParent->left; //далее аналогично
        if (newParent->left != nullptr) newParent->left->parent = node;
        newParent->parent = node->parent;
        if (node->parent == nullptr) root = newParent;
        else if (node == node->parent->left) 
            node->parent->left = newParent;
        else node->parent->right = newParent;
        newParent->left = node;
        node->parent = newParent;
    }

    void fixViolation(Node*& pt) { //исправление нарушений свойств КЧД
        Node* pt_parent = nullptr;
        Node* pt_grandparent = nullptr;
        //текущий узел не корень и родитель красный
        while ((pt != root) && (pt->color == RED) && (pt->parent->color == RED)) {
            pt_parent = pt->parent; //родитель
            pt_grandparent = pt_parent->parent; //запоминаем деда
            if (pt_parent == pt_grandparent->left) { //родитель - левый потомок деда?
                Node* pt_uncle = pt_grandparent->right; //запоминаем дядю
                if (pt_uncle != nullptr && pt_uncle->color == RED) { //дядя красный
                    pt_grandparent->color = RED; //дед становится красным
                    pt_parent->color = BLACK; //родитель становится черным
                    pt_uncle->color = BLACK; //дядя становится черным
                    pt = pt_grandparent; //переход к деду
                }
                else {
                    if (pt == pt_parent->right) { //тек узел - праый ребенок
                        rotateLeft(pt_parent); //поворот налево
                        pt = pt_parent; //обновляем текущий узел
                        pt_parent = pt->parent; //обновляем родителя
                    }
                    rotateRight(pt_grandparent); //поворот направо
                    swap(pt_parent->color, pt_grandparent->color); //смена цвета
                    pt = pt_parent; //переход к родителю
                }
            }
            else {
                Node* pt_uncle = pt_grandparent->left; //запоминаем дядю
                if ((pt_uncle != nullptr) && (pt_uncle->color == RED)) { //дядя красный
                    pt_grandparent->color = RED; //дед становится красным
                    pt_parent->color = BLACK; //родитель становится черным
                    pt_uncle->color = BLACK; //дядя становится черным
                    pt = pt_grandparent; //переход к дедушке
                }
                else {
                    if (pt == pt_parent->left) { //тек узел - левый ребенок
                        rotateRight(pt_parent); //поворот направо
                        pt = pt_parent; //обновляем текущий узел
                        pt_parent = pt->parent; //обновляем родителя
                    }
                    rotateLeft(pt_grandparent); //поворот налево
                    swap(pt_parent->color, pt_grandparent->color); //смена цвета
                    pt = pt_parent; //переход к родителю
                }
            }
        }
        root->color = BLACK; //корень всегда черный 
    }

public:
    RedBlackTree() : root(nullptr) {}

    void insert(string name) { //вставка нового узла
        Node* newNode = new Node(name); //создание нового узла
        if (root == nullptr) {
            root = newNode; //дерево пустое -> новый узел - корень
            root->color = BLACK; //корень всегда черный
            return;
        }
        Node* parent = nullptr; //родитель
        Node* current = root; //начинаем с корня
        while (current != nullptr) { //поиск места для нового узла
            parent = current; //запоминаем родителя
            if (newNode->name < current->name) current = current->left; //переход к левому поддереву
            else current = current->right; //переход к правому поддереву
        }
        newNode->parent = parent; //родитель для новго узла
        if (newNode->name < parent->name)  parent->left = newNode; //устанавливаем как левое поддерево
        else parent->right = newNode; //устанавливаем как правое поддерево
        fixViolation(newNode); //исправляем возможные нарушения
    }

    void remove(const string& key) { //удаление
        Node* nodeToDelete = search(root, key); //ищем узел с данным ключом
        if (nodeToDelete == nullptr) { //не найден
            cout << "Ключ не найден: " << key << endl;
            return;
        }
        Node* toDelete = nodeToDelete; //указатель на узел, который будет удален
        Node* child = nullptr; //указатель для хранения дочернего узла удаляемого узла
        if (nodeToDelete->left == nullptr || nodeToDelete->right == nullptr) { //какого-то ребенка
            child = nodeToDelete->left ? nodeToDelete->left : nodeToDelete->right; 
        } //присваиваем значение существующего ребенка
        else { //два ребенка
            toDelete = nodeToDelete->right; //удаляем правый узел
            //в правом поддереве ищем наименьший узел, чтобы заменить на удаляемый
            while (toDelete->left != nullptr) toDelete = toDelete->left;
            child = toDelete->left;
        }
        if (child != nullptr) { //есть дочерний узел
            child->parent = toDelete->parent; //устанавливаем его родителей todelete
        } 
        if (toDelete->parent == nullptr) { //todelete-корень
            root = child; //обновляем корень
        }
        else if (toDelete == toDelete->parent->left) { //todelete-левый
            toDelete->parent->left = child;
        }
        else toDelete->parent->right = child; //todelete-правый
        if (toDelete != nodeToDelete) { //узел для удаления не является искомым узлом
            nodeToDelete->name = toDelete->name; //копируем имя
        }
        if (toDelete->color) { //исправляем дерево (если был черным todelete)
            fixViolation(child);
        }
        delete toDelete;
    }

    Node* search(Node* node, const string& key) {
        while (node != nullptr) {
            if (key == node->name) return node; //возвращаем указатель на узел
            else if (key < node->name) { //ключ < имени узла
                node = node->left; //переход влево
            }
            else node = node->right; //иначе вправо
        }
        return nullptr; //не найден
    }

    void print(Node* t, int u) { //вывод
        if (t == nullptr) return;
        print(t->left, u + 1); //сначала левое поддерево
        string indent = ""; 
        for (int i = 0; i < u; ++i) indent += "|  ";
        string color = t->color ? "черный" : "красный";
        cout << indent  << t->name << " (" << color << ")" << endl;
        print(t->right, u + 1); //правое поддерево   
    }
    void display() {
        if (root == nullptr) {
            cout << "Дерево пустое" << endl;
            return;
        }
        print(root, 0); 
    }

    void inorder() { //симметричный обход дерева: левое поддерево -> корень (тек узел) -> правое поддерево 
        Node* current = root;
        Node* stack[100]; //стек для хранения узлов
        int top = -1; //индекс стека
        while (current != nullptr || top != -1) {
            while (current != nullptr) { //переходим в самому левому узлу от текущего
                stack[++top] = current;
                current = current->left;
            }
            current = stack[top--]; //удаляем узел из стека
            cout << current->name << " "; //выводим имя узла
            current = current->right; //переход к правому поддереву
        }
        cout << endl;
    }

    int length(const string& key) { //длина пути от корня до узла с заданным именем 
        Node* current = root;
        int length = 0;
        while (current != nullptr) {
            if (current->name == key) {
                return length; // длина пути найдена
            }
            else if (key < current->name) {
                current = current->left;
            }
            else {
                current = current->right;
            }
            length++;
        }
        return -1; //не найдено
    }
    int height() { //высота дерева
        queue<pair<Node*, int>> nodeQueue; //очередь для хранения уровня
        nodeQueue.push(make_pair(root, 0)); //корень на уровне 0
        int maxHeight = -1;
        while (!nodeQueue.empty()) {
            auto nodePair = nodeQueue.front();
            nodeQueue.pop();
            Node* node = nodePair.first;
            int height = nodePair.second;
            if (node == nullptr) continue; //пропускаем пустые узлы
            maxHeight = max(maxHeight, height); //обновляем максимальную высоту
            nodeQueue.push(make_pair(node->left, height + 1)); //левое поддерево
            nodeQueue.push(make_pair(node->right, height + 1)); //правое поддерево
        }
        return maxHeight; 
    }
    string maxLeafValue() { //макс значение среди листьев дерева
        if (root == nullptr) return ""; //дерево пустое
        queue<Node*> q; //очередь для обхода
        q.push(root);
        string maxLeaf = "";
        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            //является ли узел листом (no child)
            if (node->left == nullptr && node->right == nullptr) {
                if (node->name > maxLeaf) {
                    maxLeaf = node->name; //обновляем максимальное значение
                }
            }
            else {
                if (node->left) q.push(node->left); //добавляем левое поддерево в очередь
                if (node->right) q.push(node->right); //добавляем правое поддерево в очередь
            }
        }
        return maxLeaf;
    }
};

string generateRandomName() { //для генерации имен 
    static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static const int name_length = 5;
    string name;
    for (int i = 0; i < name_length; ++i) name += alphanum[rand() % (sizeof(alphanum) - 1)];
    return name;
}

int main() {
    setlocale(LC_ALL, "rus");
    RedBlackTree tree;
    int choice;
    string name, snm;
    while (true) {
        cout << "1. Ввести имя вручную\n";
        cout << "2. Сгенерировать случайные имена\n";
        cout << "3. Удалить имя\n";
        cout << "4. Печать дерева\n";
        cout << "5. Длина пути от корня до заданного значения\n";
        cout << "6. Высота дерева\n";
        cout << "7. Максимальное значение среди листьев дерева\n";
        cout << "0. Выход\n";
        cout << "Что хотите сделать?: ";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Введите количество (имен): ";
            int w;
            cin >> w;
            for (int i = 0; i < w; i++) {
                cout << "Введите имя: ";
                cin >> name;
                tree.insert(name);
            }
            break;
        case 2:
            cout << "Введите количество (имен): ";
            int q;
            cin >> q;
            for (int i = 0; i < q; ++i) {
                tree.insert(generateRandomName());
            }
            break;
        case 3:
            cout << "Введите имя для удаления: ";
            cin >> name;
            tree.remove(name);
            break;
        case 4:
            tree.display();
            break;
        case 5:
            cout << "Введите имя до которого нужно найти путь: ";
            cin >> snm;
            cout << "Длина пути от корня до заданного значения: " << tree.length(snm) << endl;
            break;
        case 6:
            cout << "Высота дерева: " << tree.height() << endl;
            break;
        case 7:
            cout << "Максимальное значение среди листьев дерева: " << tree.maxLeafValue() << endl;
            break;
        case 0:
            return 0;
        default:
            break;
        }
    }
}