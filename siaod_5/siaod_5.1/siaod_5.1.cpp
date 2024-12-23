#include <cstdlib> 
#include <windows.h> 
#include <bitset> 
#include <iostream> 
#include <fstream> 
#include <vector> 
#include <time.h> 
using namespace std;

void bit_1() {
    int x = 0x98;
    int maska = 0x1F;
    cout << "Исходное число: " << x << "\t";
    cout << "Исходное число в двоичном виде: " << bitset<16>(x) << endl;
    x = x | maska; //побитовое ИЛИ
    cout << "Итоговое число: " << x << "\t";
    cout << "Итоговое число в двоичном виде: " << bitset<16>(x) << endl;
    cout << "Пять младших битов установлены в значение 1";
}

void bit_2() {
    int x; //FDF, 4063
    cout << "\n\nВведите число: ";
    cin >> x;
    int maska = 0xEAF;
    cout << "Исходное число: " << x << "\t";
    cout << "Исходное число в двоичном виде: " << bitset<16>(x) << endl;
    x = x & maska; //побитовое И
    cout << "Итоговое число: " << x << "\t";
    cout << "Итоговое число в двоичном виде: " << bitset<16>(x) << endl;
    cout << "5-ой, 7-ый, 9-ый биты были обнулены";
}

void bit_3() {
    int x;
    cout << "\n\nВведите число: ";
    cin >> x;
    int number = 64; //2^6
    cout << "Исходное число: " << x << "\t";
    cout << "Исходное число в двоичном виде: " << bitset<16>(x) << endl;
    x = x << 6; //сдвиг вправо на 6 позиций
    cout << "Итоговое число: " << x << "\t";
    cout << "Итоговое число в двоичном виде: " << bitset<16>(x) << endl;
    cout << "Число было умножено на 64";
}

void bit_4() {
    int x;
    cout << "\n\nВведите число: ";
    cin >> x;
    int number = 64; //2^6
    cout << "Исходное число: " << x << "\t";
    cout << "Исходное число в двоичном виде: " << bitset<16>(x) << endl;
    x = x >> 6; //свдиг влево на 6 позиций
    cout << "Итоговое число: " << x << "\t";
    cout << "Итоговое число в двоичном виде: " << bitset<16>(x) << endl;
    cout << "Число было разделено на 64";
}

void bit_5() {
    int x;
    cout << "\n\nВведите число: ";
    cin >> x;
    int maska = 0x8000; //1000_0000_0000_0000
    int n;
    cout << "Введите бит, который хотите установить в 1 (от 1 до 16): ";
    cin >> n; //бит, который нужно установить в 1
    maska = maska >> (16 - n);  //сдвиг маски вправо на 16-n позиций
    cout << "Исходное число: " << x << "\t";
    cout << "Исходное число в двоичном виде: " << bitset<16>(x) << endl;
    x = x | maska; //побитовое ИЛИ 
    cout << "Итоговое число: " << x << "\t";
    cout << "Итоговое число в двоичном виде: " << bitset<16>(x) << endl;
    cout << n << "-й бит был установлен в 1";
}   

void bit_sort() {
    int n;
    cout << "\n\nВведите количество чисел (до 8): ";
    cin >> n; //размер массива 
    int x;
    unsigned char maska = 0x80; //1000_0000
    unsigned char arr = 0; 
    cout << "Введите массив чисел (от 0 до 7): ";
    for (int i = 0; i < n; i++) { //установка нужных битов в 1
        cin >> x;
        arr = arr | (maska >> x);                   //11010111
    }
    cout << "Отсортированный массив: ";
    for (int i = 0; i < 8; i++) {
        if (arr == (arr | maska)) {  //если на бите 1, то выводим номер бита 
            cout << i << " ";        //(так и будут выводиться сортированные числа)
        }
        maska >>= 1; //двигаем маску дальше
    }
}

void bit_sort2() {
    int n;
    cout << "\n\nВведите количество чисел (до 64): ";
    cin >> n;
    int x;
    unsigned char maska = 0x80; //1000_0000
    vector<unsigned char> arr = { 0, 0, 0, 0, 0, 0, 0, 0 }; //00000000, 00000000, ...
    cout << "Введите массив чисел (от 0 до 63): ";
    for (int i = 0; i < n; i++) { 
        cin >> x;
        arr[x / 8] = arr[x / 8] | (maska >> x % 8); //00000000 00100000
    }
    cout << "Отсортированный массив: ";
    for (int i = 0; i < 64; i++) {
        if (arr[i / 8] == (arr[i / 8] | maska)) {
            cout << i << " ";
        }
        if ((i + 1) % 8 == 0) {
            maska <<= 7;
        } else maska >>= 1;
    }
}

void bit_sort3() {
    int NUM9 = 10000000; //max число
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<unsigned char> arr;
    unsigned char maska = 1;

    time_t begin = clock();
    ifstream file("C:\\Users\\arina\\OneDrive\\Рабочий стол\\мои файлы\\дз\\doc1.txt");
    ofstream file_out("C:\\Users\\arina\\OneDrive\\Рабочий стол\\мои файлы\\дз\\doc2.txt");

    //ifstream file("C:\\Users\\arina\\OneDrive\\Рабочий стол\\мои файлы\\дз\\doc1.txt");
    //ofstream file_out("C:\\Users\\arina\\OneDrive\\Рабочий стол\\мои файлы\\дз\\doc2.txt");

    for (int i = 0; i < NUM9 / 8 + 1; i++) arr.push_back(0);
    int q;
    while (file >> q) arr[q / 8] = arr[q / 8] | maska << (q % 8);
    for (int i = 0; i < NUM9 / 8 + 1; i++) {
        for (int j = 0; j < 8; j++) {
            q = (arr[i] & maska) >> j;
            if (q == 1) {
                file_out << i * 8 + j << " ";
            }
            maska <<= 1;
        }
        maska = 1;
    }

    //при маске с 1 слева
    /*arr.resize(NUM9 / 64 + 1);
    int x = 0;
    while (file >> x) {
        arr[x / 8] = arr[x / 64] | maska << (x % 8);
    }
    file.close();
    cout << endl;
    for (int i = 0; i < NUM9; i++) {
        if ((maska | arr[i / 64]) == arr[i / 64])   //проверка на то, что тек единичка
            file_out << i << " ";                   //тогда ее позицию выводим 
        if ((i + 1) % 64 == 0) maska = maska << 63; //если маска дошла до конца
        else maska = maska >> 1;
    }*/

    file_out.close();
    time_t end = clock();
    cout << "\n\nВремя выполнения программы: " << (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "\nОбъем памяти для битового массива: " << arr.capacity() << " байт" << endl; //32768
    //макс число + макс сдвиг в ряду из 64 цифр / 8 для байт
}

int main() {
    setlocale(LC_ALL, "RUS");
    bit_1();
    bit_2();
    bit_3();
    bit_4();
    bit_5(); 
    bit_sort(); 
    bit_sort2();
    bit_sort3();
}