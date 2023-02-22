#include <string>

#ifndef DATABASE_H
#define DATABASE_H

const std::string TEMPORARY_FILE_NAME = "__temporary__.csv";
const unsigned int NUMBER_OF_DIGITS_FOR_ID = 7; // will be used to generate random ID

// structure of Product
struct Product {
    std::string category;
    std::string name;
    double price;
    std::string ID;
    unsigned int quantity;
    std::string description;
};

// colors for the text in terminal
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

class Database {
    private:
        const static int MINIMUM_CAPACITY = 10;
        int size;   // the number of elements
        int capacity;   // the number of elements that can be stored
        int startingIndex;  // the start of the list
        int endingIndex;    // the end of the list
        Product *products;   // pointer to array
        // helper methods
        void reallocateProducts();
        Product parseCSVRow(std::string row);
        std::string stringifyProduct(Product product);

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
        Product *getProducts() const;
        int getStartingIndex() const;
        void setStartingIndex(int index);
        int getEndingIndex() const;
        void setEndingIndex();

        // operators overload
        Database& operator=(const Database& db);

        // main methods
        // feature #1
        void printDatabase();

        // feature #2
        void push_back(Product product);
        void pop_front();

        // feature #3
        Product searchID(std::string id);
        void printRow(const Product &product);

        // feature #4
        void modify(std::string id);

        // feature #5
        void save(std::string fileName);
        void load(std::string fileName);

        // feature #6
        void ascendingSort();
        void descendingSort();

        // helper functions for main
        Product getProductInfo(std::string idStr = "");
        std::string generateID();
        bool uniqueID(std::string idStr);
};

// additional function to use in order to change the color of the text in the terminal
void setColor(Color color);


#endif