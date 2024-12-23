#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct MovieSession { //структура записи
    string name, movie, date, start;
    double price;
};

struct HashEntry {
    string key;         
    long record_index;    
    bool active;        
    HashEntry() : active(false) {}
};

class HashTable {
    private:
        vector<HashEntry> table;
        int capacity, size;
        void rehash();
        int hashFunc(const string& key) const;
    public:
        HashTable(int initial_capacity);
        void insert(const string& key, long record_index);
        void remove(const string& key);
        long search(const string& key);
        void addRecordToFile(const MovieSession& record, const string& filename);
        void removeRecordFromFile(long index, const string& filename);
        MovieSession readRecordFromFile(long index, const string& filename);
        long getRecordCount(const string& filename);
        int getSize() const;
};

#endif // HASH_TABLE_H
