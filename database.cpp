#include <iostream>
#include <cctype>
#include <iomanip>
#include <Windows.h>
#include "database.h"

const std::string _SEPARATOR_ = "+" + std::string(122, '-') + "+";
const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
// helper function
// std::string toUpperCase(const std::string&);

// constructors
Database::Database() {
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
        // copy products
        this->setSize(db.getSize());
        this->setCapacity(db.getCapacity());
        this->setStartingIndex(0);
        this->setEndingindex();

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
    delete[] products;
}

// getters and setters
int Database::getSize() const { return this->size; }
int Database::getCapacity() const { return this->capacity; }
void Database::setSize(int size) { this->size = size; }
void Database::setCapacity(int capacity) { this->capacity = capacity; }
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
    setStartingIndex(0);
    setEndingindex();
    products[endingIndex] = product;
}

void Database::pop_front() {
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no item in the database" << std::endl;
        setColor(BLACK);
        return;
    }
    // sendFirstElementToLast();
    setStartingIndex(getStartingIndex() + 1);
    setSize(getSize() - 1);
    setEndingindex();
    if (getStartingIndex() >= getSize()) {
        reallocateProducts();
    }
}

void Database::printDatabase() {
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no item in the database" << std::endl;
        setColor(BLACK);
        return;
    }
    setColor(MAGENTA);
    std::cout << _SEPARATOR_ << std::endl;
    std::cout << std::left << "| " << std::setw(10) << "CATEGORY"
              << "| " << std::setw(20) << "NAME"
              << "| " << std::setw(12) << "PRICE"
              << "| " << std::setw(10) << "ID"
              << "| " << std::setw(15) << "AVAILABILITY"
              << "| " << std::setw(44) << "DESCRIPTION" << "|"
              << std::endl;
    std::cout << _SEPARATOR_ << std::endl;
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        printProduct(this->products[i]);
    }
    setColor(BLACK);
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
            setColor(GREEN);
            std::cout << _SEPARATOR_ << std::endl;
            printProduct(products[i]);
            setColor(BLACK);
            return;
        }
    }
    setColor(LIGHTRED);
    std::cout << "Not found!" << std::endl;
    setColor(BLACK);
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

void Database::copyProduct(Product& destination, const Product& source) {
    destination.category = source.category;
    destination.name = source.name;
    destination.price = source.price;
    destination.ID = source.ID;
    destination.availability = source.availability;
    destination.description = source.description;
}

void Database::printProduct(const Product &product) {
    std::cout << std::left << "| " << std::setw(10) << product.category
              << "| " << std::setw(20) << product.name
              << "| " << std::setw(12) << product.price
              << "| " << std::setw(10) << product.ID
              << "| " << std::setw(15) << (product.availability ? "In stock" : "Out of stock")
              << "| " << std::setw(44) << product.description << "|"
              << std::endl;
    std::cout << _SEPARATOR_ << std::endl;
    // std::cout << product.category << "\t" << product.name << "\t\t" << product.price << "\t\t" << product.ID << "\t\t" << (product.availability ? "In stock" : "Out of stock") << "\t" << product.description << std::endl;
}

// void Database::sendFirstElementToLast() {
//     for (int i = 1; i < getSize(); i++) {
//         Product tmp = products[i-1];
//         products[i-1] = products[i];
//         products[i] = tmp;
//     }
// }

void Database::printClassInfo() {
    std::cout << this->getStartingIndex() << " " << this->getEndingIndex() << " " << this->getSize() << " " << this->getCapacity() << std::endl;
}

void setColor(Color color) {
    SetConsoleTextAttribute(h, color);
}

// std::string Database::getDataFromFieldAtIndex(int field, int index) {
//     switch (field) {
//         case 1:
//             return products[index].category;
//         case 2:
//             return products[index].name;
//         case 3:
//             return std::to_string(products[index].price);
//         case 4:
//             return products[index].ID;
//         case 5:
//             return std::to_string(products[index].availability);
//         case 6:
//             return products[index].description;
//         default:
//             return "Not a valid field";
//     }
// }

// std::string toUpperCase(const std::string &s) {
//     std::string newString;
//     for (char c: s) {
//         newString += toupper(c);
//     }
//     return newString;
// }