#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <vector>
#include <bitset>
#include <string>
#include <algorithm>
using namespace std;

struct Node { //структура для представления узла дерева Хаффмана
    char ch; //символ, хранящийся в узле
    int freq; //частота
    Node* left; //указатель на левый дочерний узел 
    Node* right; //на правый
    Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
}; //создание узла с символом и частотой

struct Compare { //сравнение узлов по частоте
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;  //сравнение частот
    }
};

//для построения кодов Хаффмана
void generateCodes(Node* root, const string& str, map<char, string>& codes) {
    if (!root) return; //узел пустой 
    if (!root->left && !root->right) { //листовой узел (символ)
        codes[root->ch] = str; //добавление кода для этого символа
    }
    generateCodes(root->left, str + "0", codes); //0 к коду для левого поддерева
    generateCodes(root->right, str + "1", codes); //1 для правого
}

//сжатие
string compressText(const string& text, map<char, string>& codes) {
    string compressedText = ""; //строка для сжатого текста
    for (char ch : text) { //по каждому символу в тексту
        compressedText += codes[ch];  //добавление кода символа в сжатый текст
    }
    return compressedText; //сжатый текст
}

//восстановление
string decompressText(const string& compressedText, map<string, char>& reverseCodes) {
    string decompressedText = ""; //строка для восст. текста
    string currentCode = ""; //хранение тек. кода
    for (char bit : compressedText) { //по каждому биту в сжатом тексте
        currentCode += bit; //+бит к тек. коду 
        if (reverseCodes.find(currentCode) != reverseCodes.end()) { //есть код в обратной таблице
            decompressedText += reverseCodes[currentCode];  //добавление символа в восст. текст
            currentCode = ""; //очищение кода для след. символа
        }
    }
    return decompressedText; //восстановленный текст
}

//для построения дерева Хаффмана и получения кодов
void buildHuffmanTree(const string& text, map<char, string>& codes, map<string, char>& reverseCodes) {
    map<char, int> frequencyMap; //подсчет частоты каждого символа
    for (char ch : text) {
        frequencyMap[ch]++;
    }
    //очередь для узлов дерева (по частоте)
    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    for (auto pair : frequencyMap) { //по частотам
        Node* node = new Node(pair.first, pair.second); //новый узел
        minHeap.push(node); //+ узел в очередь
    }
    while (minHeap.size() > 1) { //строим дерева хаффмана
        Node* left = minHeap.top(); //извлекаем узел с min частотой
        minHeap.pop();
        Node* right = minHeap.top(); //извдекаем след. узел
        minHeap.pop();

        Node* merged = new Node('\0', left->freq + right->freq);  //новый узел: два предыдущих 
        merged->left = left; //левый потомок - 1 узел
        merged->right = right; //правый потомок - 2 узел

        minHeap.push(merged); //+ объед. узел в очередь
    }
    //генерация кодов хаффмана из корня дерева
    generateCodes(minHeap.top(), "", codes);
    for (auto pair : codes) { //обратная таблица 
        reverseCodes[pair.second] = pair.first;
    } //ключ - код, значение - символ
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ifstream inputFile("input.txt"); 
    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>()); 
    inputFile.close();

    map<char, string> huffmanCodes; //таблица кодов хаффмана
    map<string, char> reverseCodes; //обратная таблица
    buildHuffmanTree(text, huffmanCodes, reverseCodes); //дерева хаффмана

    string compressedText = compressText(text, huffmanCodes);
    cout << "Сжатый текст: " << endl;
    cout << compressedText << endl;
    string decompressedText = decompressText(compressedText, reverseCodes);
    cout << "Восстановленный текст: " << endl;
    cout << decompressedText << endl;
    cout << endl;
    cout << "Размер текста: " << text.size() * 8 << endl;
    cout << compressedText.size() << endl;
    double compressionRatio = (double)compressedText.size() / (text.size() * 8) * 100; 
    cout << "Процент сжатия: " << endl << compressionRatio << "%" << endl;
}