#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bitset>
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>
using namespace std;

struct Symbol { //структура хранения символа и частоты
    string character;
    int frequency;
};

bool compareFrequency(const Symbol& a, const Symbol& b) { 
    return a.frequency > b.frequency; //сортировка символов по частоте (по убыванию)
}

//для построения кода Шеннона-Фано
void buildShannonFanoCode(vector<Symbol>& symbols, map<string, string>& codes, string code = "") {
    if (symbols.size() == 1) {
        codes[symbols[0].character] = code; //присвовение кода символу
        return;
    }
    //деление символов на две группы с ~ одинаковой общей частотой
    int totalFrequency = 0; 
    for (const auto& s : symbols) {
        totalFrequency += s.frequency; //сумма частот всех символов
    }
    int halfFrequency = 0;
    size_t splitIndex = 0;
    for (size_t i = 0; i < symbols.size(); ++i) {
        halfFrequency += symbols[i].frequency; //точка раздела по частоте
        if (halfFrequency >= totalFrequency / 2) {
            splitIndex = i + 1; //индекс деления массива
            break;
        }
    }
    //два подмассива для рекурсии
    vector<Symbol> left(symbols.begin(), symbols.begin() + splitIndex);
    vector<Symbol> right(symbols.begin() + splitIndex, symbols.end());
    buildShannonFanoCode(left, codes, code + "0"); //для левых 0 к коду
    buildShannonFanoCode(right, codes, code + "1"); //для правых 1 
}

//для сжатия текста
string compressText(const string& text, const map<string, string>& codes) {
    string compressedText;
    for (size_t i = 0; i < text.size(); ++i) {
        string character(1, text[i]);  //строка из одного символа
        compressedText += codes.at(character); //добавление кода символа в сжатый текст
    } //замена каждого символа его кодом из таблицы кодов
    return compressedText; //сжатый текст
}

//для восстановления текста
string decompressText(const string& compressedText, const map<string, string>& reverseCodes) {
    string decompressedText;
    string currentCode;
    for (char c : compressedText) {
        currentCode += c; //текущий код из симовлов сжатого текста
        if (reverseCodes.find(currentCode) != reverseCodes.end()) { //код найден в обратной таблице
            decompressedText += reverseCodes.at(currentCode); //добавление символа
            currentCode = "";
        }
    }
    return decompressedText; //востановленный текст
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ifstream inputFile("input.txt");  
    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();
    //подсчет частоты появления каждого символа
    map<string, int> frequencyMap;
    for (size_t i = 0; i < text.size(); ++i) {
        string character(1, text[i]);
        frequencyMap[character]++;
    }
    //сортировка символов по частоте
    vector<Symbol> symbols;
    for (const auto& entry : frequencyMap) {
        symbols.push_back({ entry.first, entry.second });
    } //символы с частотой 
    sort(symbols.begin(), symbols.end(), compareFrequency);

    map<string, string> codes;
    buildShannonFanoCode(symbols, codes);

    map<string, string> reverseCodes;
    for (const auto& entry : codes) {
        reverseCodes[entry.second] = entry.first;
    } //создание обратной таблицы

    string compressedText = compressText(text, codes);
    cout << "Сжатый текст: " << endl;
    for (char c : compressedText) {
        cout << c;
    } //сжатый текст в бинарном виде
    cout << endl;

    string decompressedText = decompressText(compressedText, reverseCodes);
    cout << "Восстановленный текст: " << endl;
    cout << decompressedText << endl;

    double compressionRatio = (double)compressedText.size() / (text.size() * 8) * 100;
    cout << "Процент сжатия: " << compressionRatio << "%" << endl;
}
