#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <sstream>
#include <windows.h>
using namespace std;

struct record {
    int key;
    char data[15];
};

void text_file(string file_name, int count) { //создание текстового файла
    ofstream file(file_name); //создание файла
    if (!file) {
        cout << "Ошибка при открытии файла\n";
        return;
    }
    srand(static_cast<unsigned>(time(0))); //генератор случайных чисел, значение ф. time отличается каждую секунду -> можно получить разные случайные числа
    for (int i = 0; i < count; i++) {
        int numb_city = rand() % 900000 + 100000; //код города
        string name = "City " + to_string(i + 1);
        file << numb_city << "\t" << name << endl; //в строку в файл
    }
    file << "110011" << "\t" << "City 0" << endl;
    file.close();
}

void to_bin(string text_file, string binary_file) { //преобразование в двоичный файл
    ifstream txt_file(text_file);
    if (!txt_file) {
        cout << "Файл не существует\n";
        return;
    }
    ofstream bin_file(binary_file, ios::binary); //создаем bin_file для записи в двоичный файл
    if (!bin_file) {                             //ios::binary - файл используется в двоичном режиме
        cout << "Ошибка при открытии двоичного файла." << endl;
        return;
    }
    record r;
    while (!txt_file.eof()) {
        txt_file >> r.key;
        txt_file.get();
        txt_file.getline(r.data, 15, '\n');
        //getline(txt_file, r.data);
        bin_file.write((char*)&r, sizeof(record)); 
    }
    txt_file.close();
    bin_file.close();
}

void from_bin(string file_name) { //чтение бинарного файла
    ifstream bin_file(file_name, ios::in | ios::binary);
    record r;
    while (bin_file.read((char*)&r, sizeof(record))) {
        cout << r.key << " " << r.data << "\n";
    }
    bin_file.close();
}
 
int search_bin(string file_name, int key) {
    ifstream bin_file(file_name, ios::in | ios::binary);
    if (bin_file) {
        record r;
        int start = clock();
        while (bin_file.read((char*)&r, sizeof(record))) {
            if (r.key == key) {
                cout << "Найдено искомое значение: " << r.key << " " << r.data << "\n";
                bin_file.close();
                cout << "Time: " << clock() - start << " мс\n";
                return r.key;
            }
        }
    }
    else {
        cout << "Ошибка при открытии файла\n";
    }
}


int main() {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    text_file("text_students.txt", 100); //второе значение - количество записей 
    to_bin("text_students.txt", "students.bin"); //в двоичный файл
    //from_bin("students.bin");
    cout << "Поиск: " << search_bin("students.bin", 110011);
}