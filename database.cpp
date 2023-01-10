#include <iostream>
#include <cctype>
#include <iomanip>
#include <Windows.h>
#include <stdlib.h>
#include "database.h"

// the line seperator
const std::string _SEPARATOR_ = "+" + std::string(141, '-') + "+";
// handle for setColor()
const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

// helper functions declaration
void copyProduct(Product &destination, const Product &source);
void printProduct(const Product &product);
void printSearchResult(const Product &product);
void printHeader();
// to limit the length of string in the output
std::string limitStr(std::string s, int limit);


// constructors
Database::Database() {
    // set to default values
    setSize(0);
    setCapacity(MINIMUM_CAPACITY);
    this->products = new Product[capacity];
    setStartingIndex(0);
    setEndingindex();
}

Database::Database(const Database& db) {
    *this = db;
}

// operator overload
Database& Database::operator=(const Database& db) {
    if (this != &db) {
        // copy data members

        // the new Product[2] is just there to ensure that we have something to delete later on, otherwise it would result in segmentation fault
        this->products = new Product[2];
        // set data members of the left side of operator= to have the same value as the right side of the operator
        this->setSize(db.getSize());
        this->setCapacity(db.getCapacity());
        this->setStartingIndex(0);
        this->setEndingindex();

        // a pointer to a new array of products
        Product* tmp = new Product[db.getCapacity()];

        for (int i = db.getStartingIndex(), j = 0; i <= db.getEndingIndex(); i++, j++) {
            // copy a field at a time
            copyProduct(tmp[j], db.products[i]);
        }

        // delete previous data of this object and set it to point to new data
        delete[] this->products;
        this->products = tmp;
    }
    return *this;
}

// destructor
Database::~Database() {
    // delete the data in the array
    delete[] products;
}

// getters and setters
int Database::getSize() const { return this->size; }
int Database::getCapacity() const { return this->capacity; }
void Database::setSize(int size) { this->size = size; }
void Database::setCapacity(int capacity) { this->capacity = capacity; }
Product* Database::getProducts() const { return this->products; }
int Database::getStartingIndex() const { return this->startingIndex; }
void Database::setStartingIndex(int index) { this->startingIndex = index; }
int Database::getEndingIndex() const { return this->endingIndex; }
void Database::setEndingindex() { this->endingIndex = getStartingIndex() + getSize() - 1; }

// main methods
void Database::push_back(Product product) {
    // if capacity reached, then reallocate new memory
    if (this->endingIndex + 1 == this->capacity) {
        reallocateProducts();
    }

    // at an element at index size, then increase size
    setSize(getSize() + 1);
    setEndingindex();
    products[endingIndex] = product;
}

void Database::pop_front() {
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no item in the database" << std::endl;
        setColor(LIGHTGRAY);
        return;
    }
    // sendFirstElementToLast();
    setStartingIndex(getStartingIndex() + 1);
    setSize(getSize() - 1);
    if (getStartingIndex() >= getSize()) {
        reallocateProducts();
    }
}

void Database::printDatabase() {
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no item in the database" << std::endl;
        setColor(LIGHTGRAY);
        return;
    }
    setColor(MAGENTA);
    printHeader();
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        printProduct(this->products[i]);
    }
    setColor(LIGHTGRAY);
}


// void Database::search(int field, std::string targetValue) {
//     bool found = false;
//     for (int i = 0; i < getSize(); i++) {
//         if (toUpperCase(getDataFromFieldAtIndex(field, i)) == toUpperCase(targetValue)){
//             printProduct(products[i]);
//             found = true;
//         }
//     }
//     if (!found) std::cout << "Not found" << std::endl;
// }

void Database::searchID(std::string id) {
    for (int i = getStartingIndex(); i <= getStartingIndex() + getSize(); i++) {
        if (products[i].ID == id) {
            printSearchResult(products[i]);
            return;
        }
    }
    setColor(LIGHTRED);
    std::cout << "Not found!" << std::endl;
    setColor(LIGHTGRAY);
    // search(ID, id);
}

// helper methods
void Database::reallocateProducts() {
    if (endingIndex + 1 == capacity) setCapacity(getCapacity() * 2);
    Product *tmp = new Product[getCapacity()];
    Product* current = this->products;

    for (int i = getStartingIndex(), j = 0; i <= getEndingIndex(); i++, j++) {
        copyProduct(tmp[j], current[i]);
    }

    delete[] this->products;
    this->products = tmp;
    setStartingIndex(0);
    setEndingindex();
 }

void copyProduct(Product& destination, const Product& source) {
    destination.category = source.category;
    destination.name = source.name;
    destination.price = source.price;
    destination.ID = source.ID;
    destination.quantity = source.quantity;
    destination.description = source.description;
}

void printProduct(const Product &product) {
    std::cout << std::left << std::fixed << std::setprecision(2) << "| " << std::setw(22) << limitStr(product.category, 17)
              << "| " << std::setw(26) << limitStr(product.name, 22)
              << "| " << std::setw(15) << product.price
              << "| " << std::setw(10) << product.ID
              << "| " << std::setw(10) << product.quantity
              << "| " << std::setw(47) << limitStr(product.description, 45) << "|"
              << std::endl;
    std::cout << _SEPARATOR_ << std::endl;
}

void printSearchResult(const Product &product) {
    setColor(GREEN);
    printHeader();
    printProduct(product);
    setColor(LIGHTGRAY);
}

void printHeader() {
    std::cout << _SEPARATOR_ << std::endl;
    std::cout << std::left << "| " << std::setw(22) << "CATEGORY"
                << "| " << std::setw(26) << "NAME"
                << "| " << std::setw(15) << "PRICE ($)"
                << "| " << std::setw(10) << "ID"
                << "| " << std::setw(10) << "QUANTITY"
                << "| " << std::setw(47) << "DESCRIPTION" << "|"
                << std::endl;
    std::cout << _SEPARATOR_ << std::endl;
}

void setColor(Color color) {
    SetConsoleTextAttribute(h, color);
}

std::string limitStr(std::string s, int limit) {
    if (s.length() > limit) {
        s = s.substr(0, limit);
        s += "...";
    }
    return s;
}

std::string limitDoubleLength(double num) {
    return std::to_string(num);
}