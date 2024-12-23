#include "hash_table.h"

HashTable::HashTable(int initial_capacity) : capacity(initial_capacity), size(0) {
    table.resize(capacity); //конструктор
}

int HashTable::hashFunc(const string& key) const { //хеш-функция
    int hash = 0;
    for (char ch : key) hash = (hash + static_cast<int>(ch)) % capacity;
    return hash;
}

void HashTable::rehash() { //рехеширование
    vector<HashEntry> old_table = table;
    capacity *= 2; //увеличим объем таблицы
    table.clear();
    table.resize(capacity);
    size = 0;
    for (const HashEntry& entry : old_table) {
        if (entry.active) {
            insert(entry.key, entry.record_index);
        }
    }
}

void HashTable::insert(const string& key, long record_index) { //вставка
    if (size >= capacity / 2) rehash();
    int index = hashFunc(key);
    while (table[index].active) {
        index = (index + 1) % capacity; //открытая адресация
    }
    table[index].key = key;
    table[index].record_index = record_index;
    table[index].active = true;
    size++;
}

void HashTable::remove(const string& key) { //удаление
    int index = hashFunc(key);
    while (table[index].active) {
        if (table[index].key == key) {
            table[index].active = false;
            size--;
            return;
        }
        index = (index + 1) % capacity;
    }
}

long HashTable::search(const string& key) { //поиск
    int index = hashFunc(key);
    while (table[index].active) {
        if (table[index].key == key) {
            return table[index].record_index;
        }
        index = (index + 1) % capacity;
    }
    throw runtime_error("Key not found");
}

void HashTable::addRecordToFile(const MovieSession& record, const string& filename) { //добавление записи в файл
    ofstream ofs(filename, ios::binary | ios::app);
    if (!ofs) {
        cerr << "Ошибка при открытии файла для добавления записи." << endl;
        return;
    }
    ofs.write(reinterpret_cast<const char*>(&record), sizeof(MovieSession));
    ofs.close();
}

void HashTable::removeRecordFromFile(long index, const string& filename) { //удаление записи из файла
    ifstream ifs(filename, ios::binary); //простой способ: переписываем файл без этой записи
    ofstream ofs("temp.bin", ios::binary);
    MovieSession record;
    long count = 0;
    while (ifs.read(reinterpret_cast<char*>(&record), sizeof(MovieSession))) {
        if (count != index) {
            ofs.write(reinterpret_cast<const char*>(&record), sizeof(MovieSession));
        }
        count++;
    }
    ifs.close();
    ofs.close();
    remove(filename.c_str());
    rename("temp.bin", filename.c_str());
}

MovieSession HashTable::readRecordFromFile(long index, const string& filename) { //чтение записи из файла
    ifstream ifs(filename, ios::binary);
    MovieSession record;
    ifs.seekg(index * sizeof(MovieSession));
    ifs.read(reinterpret_cast<char*>(&record), sizeof(MovieSession));
    ifs.close();
    return record;
}

long HashTable::getRecordCount(const string& filename) { //получение количества записей
    ifstream ifs(filename, ios::binary);
    if (!ifs) return 0;
    ifs.seekg(0, ios::end);
    long count = ifs.tellg() / sizeof(MovieSession);
    ifs.close();
    return count;
}

int HashTable::getSize() const { //получение размера таблицы
    return size;
}