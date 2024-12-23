#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
using namespace std;
const int INF = numeric_limits<int>::max();

class Graph {
private:
    int vertices; //кол-во вершин в графе
    vector<vector<int>> adjacency_matrix; //матрица смежности для представления графа
   
    int find(int v, vector<int>& parent) { //поиск корня вершины
        if (parent[v] != v) {
            parent[v] = find(parent[v], parent);
        } //рекурсивно ищется корень и обновляется родительский узел
        return parent[v];
    }

    void unite(int v1, int v2, vector<int>& parent) { //объединение двух вершин
        parent[find(v1, parent)] = find(v2, parent);
    } //находит корни для обеих вершин и устанавливает родителя v1 как родителя v2

public:
    Graph(int vertices) { //конструктор
        this->vertices = vertices; //устанавливаем кол-вол вершин 
        adjacency_matrix.resize(vertices, vector<int>(vertices, 0)); //инициализируем матрицу нулями
    }
    
    void addEdge(int u, int v, int weight) { //добавление одного ребра
        if (u <= vertices && v <= vertices && u > 0 && v > 0) {
            adjacency_matrix[u - 1][v - 1] = weight; //индексация с 0
            adjacency_matrix[v - 1][u - 1] = weight; //для неориентированного графа
        }
        else {
            cout << "Некорректные вершины!\n";
        }
    }

    void addEdgesBulk(int edges) { //для массового ввода ребер
        cout << "Введите рёбра в формате 'начало конец вес':\n";
        for (int i = 0; i < edges; ++i) {
            int u, v, weight;
            cout << "Ребро " << i + 1 << ": ";
            cin >> u >> v >> weight;
            addEdge(u, v, weight);
        }
    }

    void display() { //вывод
        for (int i = 0; i < vertices; i++) { //по строкам матрицы
            for (int j = 0; j < vertices; j++) { //по столбцам
                cout << adjacency_matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    
    int find_center() {
        for (int k = 0; k < vertices; k++) {
            for (int i = 0; i < vertices; i++) {
                for (int j = 0; j < vertices; j++) {
                    if (adjacency_matrix[i][k] < INF && adjacency_matrix[k][j] < INF) {
                        adjacency_matrix[i][j] = min(adjacency_matrix[i][j],
                            adjacency_matrix[i][k] + adjacency_matrix[k][j]);
                    }
                }
            }
        }
        vector<int> max_distances(vertices);
        for (int i = 0; i < vertices; i++) {
            max_distances[i] = *max_element(adjacency_matrix[i].begin(), adjacency_matrix[i].end());
        }
        int center_index = distance(max_distances.begin(), min_element(max_distances.begin(), max_distances.end()));
        return center_index;
    }

    pair<vector<pair<int, int>>, int> kruskal() { //алгоритм Крускала
        vector<int> parent(vertices); //для хранения родителей вершин
        for (int i = 0; i < vertices; i++) { //инициализируем родителей, 
            parent[i] = i;         //каждая вершина — родитель сама себе
        }
        vector<pair<int, pair<int, int>>> edges; //для хранения всех ребер
        for (int i = 0; i < vertices; i++) { //проходим по всем вершинам
            for (int j = i; j < vertices; j++) { //по всем остальным вершинам
                if (adjacency_matrix[i][j] != 0) { //есть ли ребро
                    edges.push_back({ adjacency_matrix[i][j], {i, j} }); //добавляем ребро с весом в вектор
                }
            }
        }
        sort(edges.begin(), edges.end()); //сортируем ребра по весу
        int mst_weight = 0; //общий вес остовного дерева
        vector<pair<int, int>> mst_edges; //для хранения ребер остовного дерева
        for (auto edge : edges) { //по всем ребрам
            int weight = edge.first; //вес ребра
            int start = edge.second.first; //стартовая вершина
            int end = edge.second.second;  //конечная вершина
            if (find(start, parent) != find(end, parent)) { //если вершины разных компонентов
                unite(start, end, parent); //объединям их
                mst_weight += weight;//общий вес +
                mst_edges.push_back({ start, end }); //добавляем ребро в остовное дерево
            }
        }
        return { mst_edges, mst_weight }; //ребра и вес остовного дерева
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    int vertices;
    cout << "Количество вершин в графе: ";
    cin >> vertices;
    Graph graph(vertices);
    int choice;
    do {
        cout << "\n1. Добавить одно ребро\n";
        cout << "2. Добавить несколько рёбер\n";
        cout << "3. Найти центр графа\n";
        cout << "4. Реализация алгоритма Крускала построения остовного дерева минимального веса\n";
        cout << "5. Вывести матрицу смежности\n";
        cout << "0. Выход\n";
        cout << "Что хотите сделать?: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            int u, v, weight;
            cout << "Введите ребро (начало конец вес): ";
            cin >> u >> v >> weight;
            graph.addEdge(u, v, weight);
            break;
        }
        case 2: {
            int edges;
            cout << "Введите количество ребер: ";
            cin >> edges;
            graph.addEdgesBulk(edges);
            break;
        }
        case 3: {
            int center = graph.find_center();
            cout << "Центр графа: вершина " << center + 1 << endl;
            break;
        }
        case 4: {
            cout << "Алгоритм Крускала\n";
            pair<vector<pair<int, int>>, int> result = graph.kruskal();
            vector<pair<int, int>> mst_edges = result.first;
            int mst_weight = result.second;
            cout << "\nРебра остовного дерева (минимальный вес):\n";
            for (const auto& edge : mst_edges) {
                int start = edge.first;
                int end = edge.second;
                cout << start << " - " << end << endl;
            }
            cout << "Общий вес остовного дерева: " << mst_weight << endl;
            break;
        }
        case 5: {
            graph.display();
            break;
        }
        case 0:
            break;
        default:
            cout << "Попробуйте снова\n";
        }
    } while (choice != 0);
}

