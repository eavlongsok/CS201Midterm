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

enum fields {
    category = 1,
    name,
    price,
    ID,
    availability,
    description
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
        void sendFirstElementToLast();
        std::string getDataFromFieldAtIndex(int field, int index);

       public:
        // constructor
        Database();
        Database(const Database& db);

        // destructor
        ~Database();

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
        void search(int field, std::string targetValue);
        void searchID(std::string id);
        void printClassInfo(); // temporary, for testing purposes
};

#endif