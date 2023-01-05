#include <iostream>
#include <cctype>
#include "database.h"

std::string toUpperCase(const std::string&);

// constructors
Database::Database() {
    setSize(0);
    setCapacity(MINIMUM_CAPACITY);
    this->products = new Product[capacity];
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

        Product* tmp = new Product[db.getCapacity()];

        for (int i = 0; i < db.getSize(); i++) {
            // copy a field at a time
            copyProduct(tmp[i], db.products[i]);
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

// main methods
void Database::push_back(Product product) {
    // if capacity reached, then reallocate new memory
    if (this->size == this->capacity) {
        reallocateProducts();
    }

    // at an element at index size, then increase size
    products[size] = product;
    setSize(getSize() + 1);
}

void Database::pop_front() {
    Product product = products[getSize() - 1];
    sendFirstElementToLast();
    setSize(getSize() - 1);
}

void Database::printDatabase() {
    for (int i = 0; i < this->getSize(); i++) {
        printProduct(this->products[i]);
    }
}

void Database::search(int field, std::string targetValue) {
    bool found = false;
    for (int i = 0; i < getSize(); i++) {
        if (toUpperCase(getDataFromFieldAtIndex(field, i)) == toUpperCase(targetValue)){
            printProduct(products[i]);
            found = true;
        }
    }
    if (!found) std::cout << "Not found" << std::endl;
}

void Database::searchID(std::string id) {
    search(ID, id);
}

// helper methods
void Database::reallocateProducts() {
    setCapacity(getCapacity() * 2);
    Product *tmp = new Product[getCapacity()];
    Product* current = this->products;

    for (int i = 0; i < this->getSize(); i++) {
        copyProduct(tmp[i], current[i]);
    }

    delete[] this->products;
    this->products = tmp;
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
    std::cout << product.category << "\t" << product.name << "\t\t" << product.price << "\t\t" << product.ID << "\t\t" << (product.availability ? "In stock" : "Out of stock") << "\t" << product.description << std::endl;
}

void Database::sendFirstElementToLast() {
    for (int i = 1; i < getSize(); i++) {
        Product tmp = products[i-1];
        products[i-1] = products[i];
        products[i] = tmp;
    }
}

void Database::printClassInfo() {
    std::cout << this->getSize() << " " << this->getCapacity() << std::endl;
}

std::string Database::getDataFromFieldAtIndex(int field, int index) {
    switch (field) {
        case 1:
            return products[index].category;
        case 2:
            return products[index].name;
        case 3:
            return std::to_string(products[index].price);
        case 4:
            return products[index].ID;
        case 5:
            return std::to_string(products[index].availability);
        case 6:
            return products[index].description;
        default:
            return "Not a valid field";
    }
}





std::string toUpperCase(const std::string &s) {
    std::string newString;
    for (char c: s) {
        newString += toupper(c);
    }
    return newString;
}