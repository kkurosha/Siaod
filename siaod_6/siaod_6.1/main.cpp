#include <iostream>
#include <vector>
#include <functional> //для хеширования строк
#include <string>
using namespace std;

const int table_size = 10; //размер хеш-таблицы

struct MovieSession { //структура записи
    string name, movie, date, start;
    double price;
};

struct HashEntry { //структура элемента хеш-таблицы
    string key; //ключ записи 
    bool active; //статус занятости для коллизий
    MovieSession session; //данные о сеансе в кинотеатре
    //конструктор для значений по умолчанию
    HashEntry() : key(""), active(false) {}
};

struct HashTable { //структура хеш-таблицы 
    HashEntry* table;
    int table_size;
    HashTable(int size) { //конструктор по умолчанию
        table_size = size;
        table = new HashEntry[table_size];
    }
    ~HashTable() { delete[] table; } //деструктор
};

int hashFunc(const string& key) { //хеш-функция
    //для каждого символа в ключе добавляем его к hash и берём остаток от деления на table_size.
    int hash = 0;
    for (char ch : key) hash = (hash + ch);
    hash = hash % table_size;
    return hash;
}

void insert(HashTable& ht, const string& key, const MovieSession& session) { //вставка ключа в таблицу
    int hashIndex = hashFunc(key); //вычисляем индекс для вставки с помощью хеш-функции
    while (ht.table[hashIndex].active) { //ищем свободное место с учетом коллизий
        hashIndex = (hashIndex + 1) % table_size; //смещение на 1 (коллизия)
    }
    ht.table[hashIndex].key = key; //если место свободно вставляем элемент
    ht.table[hashIndex].session = session;
    ht.table[hashIndex].active = true;
    cout << "Вставлен элемент " << key << " Индекс: " << hashIndex << endl;
}

bool search(const HashTable& ht, const string& key) { //поиск ключа в таблице
    int hashIndex = hashFunc(key);
    while (ht.table[hashIndex].active) { //если элемент занят
        if (ht.table[hashIndex].key == key) { //если ключ совпадает
            cout << "Найден элемент " << key << " Индекс: " << hashIndex << endl;
            return true;
        }
        hashIndex = (hashIndex + 1) % table_size; //смещение на 1
    }
    cout << "Элемент " << key << " не найден" << endl;
    return false;
}

void remove(HashTable& ht, const string& key) { //удаление ключа из таблицы
    int hashIndex = hashFunc(key);
    while (ht.table[hashIndex].active) {
        if (ht.table[hashIndex].key == key) {
            ht.table[hashIndex].active = false; //удаляем запись
            cout << "Удален элемент " << key << " Индекс: " << hashIndex << endl;
            return;
        }
        hashIndex = (hashIndex + 1) % table_size; //смещение на 1
    }
    cout << "Элемент " << key << " не был найден для удаления" << endl;
}

void rehash(HashTable& ht) { //рехеширование хеш-таблицы
    int new_size = ht.table_size * 2; //увеличиваем размер
    HashTable newTable(new_size); //создаем новую таблицу
    for (int i = 0; i < ht.table_size; i++) {
        if (ht.table[i].active) {
            //вставляем все существующие элементы в новую таблицу
            insert(newTable, ht.table[i].key, ht.table[i].session);
        }
    }
    delete[] ht.table; //освобождаем старую память
    ht.table = newTable.table; //смена хеш-таблицы
    ht.table_size = new_size;
    newTable.table = nullptr; //предотвращаем двойное освобождение памяти 
    //(если ~HashTable пытается освободить область памяти, которая была уже освобождена)
    cout << "\nРехеширование было выполнено";
}

void printTable(const HashTable& ht, int s) { //вывод для наглядности
    int ss = s;
    cout << "Хеш-таблица:\n";
    for (int i = 0; i < ss; ++i) {
        if (ht.table[i].active) {
            cout << i << ") "
                << "Кинотеатр: " << ht.table[i].key << ", "
                << "Фильм: " << ht.table[i].session.movie << ", "
                << "Дата: " << ht.table[i].session.date << ", "
                << "Время: " << ht.table[i].session.start << ", "
                << "Цена: " << ht.table[i].session.price << "\n";
        }
        else {
            cout << i << ") (пусто)\n";
        }
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "RUS");
    int s = 10; //размер таблицы
    HashTable ht(s);
    printTable(ht, s);
    cout << "------------------------------------\n\n";

    //пример данных
    MovieSession session1 = { "ADCADC", "Movie A", "01.10.2024", "09:00", 10.0 };
    MovieSession session2 = { "zxc", "Movie B", "02.10.2024", "16:30", 12.0 };
    MovieSession session3 = { "xzxc", "Movie C", "03.10.2024", "21:00", 15.0 };
    MovieSession session4 = { "qwerty", "Movie A", "03.10.2024", "11:00", 20.0 };

    //вставка ключей без коллизии
    insert(ht, session1.name, session1);
    insert(ht, session4.name, session4);
    cout << "\n";
    printTable(ht, s);
    cout << "------------------------------------\n\n";

    //поиск ключей
    search(ht, "ADCADC");
    search(ht, "xzxc");
    cout << "\n";
    cout << "------------------------------------\n\n";

    //удаление ключа
    remove(ht, "qwerty");
    cout << "\n";
    printTable(ht, s);
    cout << "------------------------------------\n\n";

    //коллизия
    cout << "Индекс zxc, полученный с помощью хеш-функции: " << hashFunc("zxc") << endl;
    cout << "Индекс xzxc, полученный с помощью хеш-функции: " << hashFunc("xzxc") << endl;
    insert(ht, session2.name, session2); 
    insert(ht, session3.name, session3);
    printTable(ht, s);
    cout << "------------------------------------\n\n";


    remove(ht, "zxc");
    cout << "\n";
    printTable(ht, s);
    cout << "------------------------------------\n\n";

    search(ht, "xzxc");
    cout << "\n";
    cout << "------------------------------------\n\n";

    //рехеширование
    rehash(ht);
    cout << "\n";
    printTable(ht, s * 2);
    cout << "------------------------------------\n\n";

    /*cout << "zxc " << hashFunc("zxc") << endl;
    cout << "xzxc " << hashFunc("xzxc") << endl;*/
}