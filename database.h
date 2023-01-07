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

enum Color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE,
};

// enum fields {
//     category = 1,
//     name,
//     price,
//     ID,
//     availability,
//     description
// };

class Database {
    private:
        const static int MINIMUM_CAPACITY = 1;
        int size;
        int capacity;
        int startingIndex;
        int endingIndex;
        Product *products;   // pointer to array
        // helper methods
        void reallocateProducts();
        static void copyProduct(Product &destination, const Product &source);
        static void printProduct(const Product &product);
        // void sendFirstElementToLast();
        // std::string getDataFromFieldAtIndex(int field, int index);

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
        int getStartingIndex() const;
        void setStartingIndex(int index);
        int getEndingIndex() const;
        void setEndingindex();

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

void setColor(Color color);
#endif