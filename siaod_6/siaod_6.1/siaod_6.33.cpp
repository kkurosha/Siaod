#include "hash_table.h"

int main() {
    setlocale(LC_ALL, "rus");
    HashTable hashTable(10);
    string filename = "sessions.bin";

    //создаем записи
    MovieSession record1 = { "ADCADC", "Movie A", "01.10.2024", "09:00", 10.0 };
    hashTable.addRecordToFile(record1, filename);
    hashTable.insert(record1.name, hashTable.getRecordCount(filename) - 1);
    MovieSession record2 = { "zxc", "Movie B", "02.10.2024", "16:30", 12.0 };
    hashTable.addRecordToFile(record2, filename);
    hashTable.insert(record2.name, hashTable.getRecordCount(filename) - 1);
    MovieSession record3 = { "xzxc", "Movie C", "03.10.2024", "21:00", 15.0 };
    hashTable.addRecordToFile(record3, filename);
    hashTable.insert(record3.name, hashTable.getRecordCount(filename) - 1);

    cout << "Количество записей в файле: " << hashTable.getRecordCount(filename) << endl;

    //удаляем запись
    try {
        hashTable.remove("ADCADC");
        hashTable.removeRecordFromFile(1, filename); //удаляем запись из файла
        cout << "Запись под ключом ADCADC удалена \n";
    }
    catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    //ищем запись
    try {
        long index = hashTable.search("xzxc");
        MovieSession foundRecord = hashTable.readRecordFromFile(index, filename);
        cout << "Найден: Кинотеатр: " << foundRecord.name << ", Фильм: " << foundRecord.movie << ", Дата: "
            << foundRecord.date << ", Время: " << foundRecord.start << ", Цена: " << foundRecord.price << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
    
}
