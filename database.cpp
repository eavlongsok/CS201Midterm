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
    setEndingIndex();
}

// for copying
Database::Database(const Database& db) {
    *this = db;
}

// operator overload (copy)
Database& Database::operator=(const Database& db) {
    if (this != &db) {
        // copy data members

        // the new Product[2] is just there to ensure that we have something to delete later on, otherwise it would result in segmentation fault
        this->products = new Product[2];
        // set data members of the left side of operator= to have the same value as the right side of the operator
        this->setSize(db.getSize());
        this->setCapacity(db.getCapacity());
        this->setStartingIndex(0);
        this->setEndingIndex();

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
// no argument for setEndingIndex, it just does some basic calculation
void Database::setEndingIndex() { this->endingIndex = getStartingIndex() + getSize() - 1; }


void Database::printDatabase() {
    // if database is empty, return
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no item in the database" << std::endl;
        setColor(LIGHTGRAY);
        return;
    }
    // else print the header, then the rows
    setColor(MAGENTA);
    printHeader();
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        printProduct(this->products[i]);
    }
    setColor(LIGHTGRAY);
}

// main methods
void Database::push_back(Product product) {
    // if capacity reached, then reallocate to new memory block
    if (this->endingIndex + 1 == this->capacity) {
        reallocateProducts();
    }
    setSize(getSize() + 1);
    setEndingIndex();
    products[endingIndex] = product;
}

void Database::pop_front() {
    // if there's nothing in the database, return
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no item in the database" << std::endl;
        setColor(LIGHTGRAY);
        return;
    }
    // else we increase the starting index, and decrease size
    setSize(getSize() - 1);
    setStartingIndex(getStartingIndex() + 1);
    // if the number of deleted elements exceeds the size, then reallocate to new memory block
    if (getStartingIndex() + 1 >= getSize()) {
        reallocateProducts();
    }
    setColor(GREEN);
    std::cout << "Deleted the first item in the database" << std::endl;
    setColor(LIGHTGRAY);
}


void Database::searchID(std::string id) {
    // search all products in database, if the ID match, then print that product
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        if (products[i].ID == id) {
            printSearchResult(products[i]);
            return;
        }
    }
    // if not found, print "not found"
    setColor(LIGHTRED);
    std::cout << "Not found!" << std::endl;
    setColor(LIGHTGRAY);
}

// helper methods/functions
void Database::reallocateProducts() {
    // if there are no more space for new products, then double the capacity
    if (endingIndex + 1 == capacity) setCapacity(getCapacity() * 2);
    // tmp points to the head of the new array, and current points for the current array head
    Product *tmp = new Product[getCapacity()];
    Product *current = this->products;

    for (int i = getStartingIndex(), j = 0; i <= getEndingIndex(); i++, j++) {
        // copy the product one by one, from current to tmp
        copyProduct(tmp[j], current[i]);
    }

    // delete the products in the current array
    delete[] this->products;
    // set the head of the array to point to the new array (tmp)
    this->products = tmp;
    // set the starting index to 0, and calculate the ending index
    setStartingIndex(0);
    setEndingIndex();
 }

void copyProduct(Product& destination, const Product& source) {
    // copy the product information
    destination.category = source.category;
    destination.name = source.name;
    destination.price = source.price;
    destination.ID = source.ID;
    destination.quantity = source.quantity;
    destination.description = source.description;
}

void printHeader() {
    std::cout << _SEPARATOR_ << std::endl;
    std::cout << std::left << "| " << std::setw(10) << "ID"
                << "| " << std::setw(22) << "CATEGORY"
                << "| " << std::setw(26) << "NAME"
                << "| " << std::setw(15) << "PRICE ($)"
                << "| " << std::setw(10) << "QUANTITY"
                << "| " << std::setw(47) << "DESCRIPTION" << "|"
                << std::endl;
    std::cout << _SEPARATOR_ << std::endl;
}

void printProduct(const Product &product) {
    std::cout << std::left << std::fixed << std::setprecision(2) << "| " << std::setw(10) << product.ID
              << "| " << std::setw(22) << limitStr(product.category, 17)
              << "| " << std::setw(26) << limitStr(product.name, 22)
              << "| " << std::setw(15) << product.price
              << "| " << std::setw(10) << product.quantity
              << "| " << std::setw(47) << limitStr(product.description, 44) << "|"
              << std::endl;
    std::cout << _SEPARATOR_ << std::endl;
}

// used only for search function
void printSearchResult(const Product &product) {
    setColor(GREEN);
    printHeader();
    printProduct(product);
    setColor(LIGHTGRAY);
}


void setColor(Color color) {
    // set the text in the concole to matcht the color
    SetConsoleTextAttribute(h, color);
}

std::string limitStr(std::string s, int limit) {
    // if the length of the string exceeds the limit, then only leave the characters from index 0 to limit - 1, then add "..." to the string
    if (s.length() > limit) {
        s = s.substr(0, limit);
        s += "...";
    }
    return s;
}