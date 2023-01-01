#include <string>
#ifndef DATABASE_H
#define DATABASE_H

const int MINIMUM_ARR_SIZE = 10;

struct Product {
    std::string category;
    std::string name;
    double price;
    std::string ID;
    bool availability;
    std::string description; // might remove later
};

class Database {
    private:
        int size;
        int capacity;
        Product *products; // pointer to array

    public:
        Database();
        Database(int size);
        void push_back();
        void pop_front();
        void printDatabase();
        void search(std::string field, std::string targetValue);
        void searchID(std::string id);
};

#endif