#include <iostream> 
using namespace std; 

struct Node { //структурa для узла дерева
    int key; //инфорационное поле - целое число 
    Node* left; //левое поддерево
    Node* right; //правое поддерево
    Node(int val) : key(val), left(nullptr), right(nullptr) {} //конструктор 
};

class BinarySearchTree { //класс для бинарного дерева поиска
private:
    Node* root; //корень дерева

public:
    BinarySearchTree() : root(nullptr) {} //конструктор для создания пустого дерева

    void insert(int key) { //добавление узла
        Node* newNode = new Node(key); //cоздаем новый узел
        if (root == nullptr) { //если дерево пустое, то новый узел - корень
            root = newNode;
            return;
        }
        Node* current = root; //для перемещения по дереву
        Node* parent = nullptr; //родитель
        while (current != nullptr) { //ищем место для нового узла
            parent = current; //запоминаем родителя
            if (key < current->key) { current = current->left; } //переход в левое поддерево
            else { current = current->right; } //в правое поддерево 
        }
        if (key < parent->key) { parent->left = newNode; } //добавляем в левое поддерево
        else { parent->right = newNode; } //добавляем в правое поддерево
    }

    int search(int key) { //поиск по ключу
        Node* current = root; //для перемещения по дереву
        while (current != nullptr) { //пока не достигли конца дерева
            if (current->key == key) { return current->key; } //ключ найден
            else if (key < current->key) { current = current->left; } //поиск слева
            else { current = current->right; } //поиск справа
        }
        return -1; //не найден ключ 
    }

    void remove(int key) { //удаление
        Node* current = root; //для перемещения по дереву
        Node* parent = nullptr; //родитель
        while (current != nullptr && current->key != key) { //ищем узел для удаления
            parent = current; //запоминаем родителя
            if (key < current->key) current = current->left; //двигаемся влево
            else current = current->right; //двигаемся вправо 
        }
        if (current == nullptr) return; //узел не найден
        //узел не имеет поддеревьев
        if (current->left == nullptr && current->right == nullptr) {
            if (current == root) root = nullptr; //узел - корень
            else if (parent->left == current) parent->left = nullptr; //узел - левый ребенок
            else parent->right = nullptr; //узел - правый ребенок
        }
        //узел имеет одно поддерево
        else if (current->left == nullptr || current->right == nullptr) {
            Node* child = (current->left != nullptr) ? current->left : current->right; //ребенок
            if (current == root) root = child; //если удаляем корень
            else if (parent->left == current) parent->left = child; //узел - левый ребенок
            else parent->right = child; //узел - правый ребенок
        }
        //узел имеет два поддерева
        else { //ищем наименьший узел в правом поддереве
            Node* successor = current->right; //начинаем с правого поддерева
            Node* successorParent = current; //родитель для узла-наследника
            while (successor->left != nullptr) { //ищем наименьший узел
                successorParent = successor;
                successor = successor->left;
            }
            current->key = successor->key; //копируем значение узла-наследника
            if (successorParent->left == successor) { //удаляем узел-наследник
                successorParent->left = successor->right; //подключаем правое поддерево
            }
            else successorParent->right = successor->right; //подключаем правое поддерево
            current = successor; //текущий узел указывает на узел-наследник
        }
        delete current; //удаляем узел
    }
    /*void display() { //отображение дерева
        Node* current = root;
        if (current == nullptr) {
            cout << "Дерево пустое\n";
            return;
        } //стек для итеративного обхода (прямой обход)
        Node* stack[100]; //стек для хранения узлов
        int top = -1; //индекс верхнего элемента стека
        stack[++top] = current; //добавляем корень в стек
        while (top >= 0) { // стек не пуст
            current = stack[top--]; //извлекаем верхний элемент
            cout << current->key << " "; //вывод значения узла
            if (current->right != nullptr) { stack[++top] = current->right; } //добавляем правое дерево
            if (current->left != nullptr) { stack[++top] = current->left; } //добавляем левое дерево
        }
        cout << endl; 
    }*/
    int height() { //высота дерева 
        if (root == nullptr) return -1; //дерево пустое
        int h = 0; //высота
        Node* queue[100]; //очередь для выполнения обхода
        int front = -1, rear = -1; //индексы вершины и конца очереди
        queue[++rear] = root; //добавляем корень в очередь
        while (front != rear) {
            int count = rear - front; 
            h++; 
            while (count--) { 
                Node* node = queue[++front]; //извлекаем узел из очереди
                if (node->left != nullptr) queue[++rear] = node->left; //добавляем левого ребенка в очередь
                if (node->right != nullptr) queue[++rear] = node->right; //добавляем правого ребенка в очередь
            }
        }
        return h; 
    }

    int edgeLength() { //длина пути (кол-во ребер)
        if (root == nullptr) return 0; //дерево пустое
        int length = 0; //длина
        Node* queue[100]; //очередь для обхода
        int front = -1, rear = -1; //индексы вершины и конца очереди
        queue[++rear] = root; //добавляем корень в очередь
        while (front != rear) {
            int count = rear - front; 
            while (count--) { 
                Node* node = queue[++front]; //извлекаем узел из очереди
                if (node->left != nullptr) {
                    queue[++rear] = node->left; //добавляем левого ребенка в очередь
                    length++; //увеличиваем длину
                }
                if (node->right != nullptr) {
                    queue[++rear] = node->right; //добавляем правого ребенка в очередь
                    length++; //увеличиваем длину
                }
            }
        }
        return length; 
    }

    double average() { //среднее арифметическое
        if (root == nullptr) return 0; //дерево пустое
        int sum = 0; //для хранения суммы всех узлов
        int count = 0; //для хранения количества узлов
        Node* queue[100]; //очередь для обхода
        int front = -1; //индекс передней части очереди
        int rear = -1; //индекс задней части очереди
        queue[++rear] = root; //добавляем корень дерева в очередь
        while (front != rear) {
            Node* node = queue[++front]; //извлекаем узел из очереди
            sum += node->key; //добавляем значение узла к сумме
            count++; //увеличиваем счетчик узлов
            //добавляем детей узла в очередь
            if (node->left != nullptr) queue[++rear] = node->left; //левое поддерево
            if (node->right != nullptr) queue[++rear] = node->right; //правое поддерево
        }
        return count != 0 ? (double)sum / count : 0; 
    }

    void print(Node* t, int u) {
        if (t == nullptr) return;
        print(t->left, u + 1); //обход левого поддерева
        for (int i = 0; i < u; ++i) { cout << "| "; }
        cout << t->key << endl; 
        print(t->right, u + 1); //обход правого поддерева
    }
    void display() { //вывод
        if (root == nullptr) {
            cout << "Дерево пустое" << endl;
            return;
        }
        print(root, 0); //начало с корня
    }
};

int main() {
    setlocale(LC_ALL, "rus"); 
    BinarySearchTree tree;
    int n; //размер
    int r;
    cout << "Как заполнить дерево? Вручную - 1, рандомно - 2: ";
    cin >> r;
    cout << "\nВставка элементов в дерево\nВведите количество элементов: ";
    cin >> n;
    switch (r) {
    case 1:
        int q;
        cout << "Введите элементы для вставки:\n";
        for (int i = 0; i < n; i++) {
            cin >> q;
            tree.insert(q);
        }
        break;
    case 2:
        srand(time(0));
        for (int i = 0; i < n; ++i) {
            int randomValue = rand() % 1000;
            tree.insert(randomValue);
        }
        break;
    }
    cout << "Дерево:\n";
    tree.display();
    cout << "Среднее арифметическое всех чисел в дереве: " << tree.average() << endl;
    cout << "Длина пути дерева (количество ребер): " << tree.edgeLength() << endl;
    cout << "Высота дерева: " << tree.height() << endl;
    cout << "Поиск по ключу: " << tree.search(40);
    cout << "\nКакой элемент удалить? ";
    int w;
    cin >> w;
    tree.remove(w);
    cout << "Дерево после удаления:\n";
    tree.display();
}
//5 4 6 1 2