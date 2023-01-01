#ifndef DATABASE_H
#define DATABASE_H

#include <string>

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
        const static int MINIMUM_CAPACITY = 10;
        int size;
        int capacity;
        Product *products;   // pointer to array
        // helper methods
        void reallocateProducts();
        static void copyProduct(Product &source, const Product &destination);
        void printProduct(const Product &product);

    public:
        // constructor
        Database();
        Database(const Database& db);

        // getters and setters
        int getSize() const;
        int getCapacity() const ;
        void setSize(int size);
        void setCapacity(int capacity);

        // operators overload
        Database& operator=(const Database& db);

        // main methods
        void push_back(Product product);
        void pop_front();
        void printDatabase();
        void search(std::string field, std::string targetValue);
        void searchID(std::string id);
        void printClassInfo(); // temporary, for testing purposes
};

#endif