#include <iostream>
#include <cctype>
#include <iomanip>
#include <limits>
#include <Windows.h>
#include <stdlib.h>
#include <fstream>
#include "database.h"

// the line seperator
const std::string _SEPARATOR_ = "+" + std::string(141, '-') + "+";
// handle for setColor()
const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

// helper functions declaration
void copyProduct(Product &destination, const Product &source);
void printProduct(const Product &product);
void printHeader();
// to limit the length of string in the output
std::string limitStr(std::string s, int limit);
void swapPointer(Database* a, Database* b);
std::string toLowerCase(std::string s);
void displayModifyMenu();

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
        std::cout << "There are no item in the database" << std::endl << std::endl;
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
    system("cls");
    setColor(GREEN);
    std::cout << "Deleted the first item in the database" << std::endl << std::endl;
    setColor(LIGHTGRAY);
}

Product Database::searchID(std::string id) {
    // search all products in database, if the ID match, then print that product
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        if (products[i].ID == id) {
            return products[i];
        }
    }
    // if not found, throw invalid_argument exception

    throw std::invalid_argument("Product Not Found");
}

void Database::modify(std::string id){
    Product product = searchID(id);
    system("cls");
    printRow(product);
    displayModifyMenu();
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            std::cout << "Enter the new Category: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(std::cin, product.category);
            break;

        case 2:
            std::cout << "Enter the new Name: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(std::cin, product.name);
            break;

        case 3:
            std::cout << "Enter the new Price: ";
            std::cin >> product.price;
            break;

        case 4:
            std::cout << "Enter the new Quantity: ";
            std::cin >> product.quantity;
            break;

        case 5:
            std::cout << "Enter the new Description: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(std::cin, product.description);
            break;

        case 6:
            std::cout << "Enter the new Category: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(std::cin, product.category);
            std::cout << "Enter the new Name: ";
            getline(std::cin, product.name);
            std::cout << "Enter the new Price: ";
            std::cin >> product.price;
            std::cout << "Enter the new Quantity: ";
            std::cin >> product.quantity;
            std::cout << "Enter the new Description: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(std::cin, product.description);
            break;

        default:
            setColor(LIGHTRED);
            std::cout << "Invalid choice!" << std::endl;
            setColor(LIGHTGRAY);
            break;
    }

    for (int i = getStartingIndex(); i<= getEndingIndex(); i++){
        if (products[i].ID == id){
            products[i] = product;
            printRow(product);
            return;
        }
    }
}

void Database::save(std::string fileName) {
    if (this->getSize() == 0) {
        setColor(RED);
        std::cout << "The database is empty. Please add some data to save" << std::endl;
        setColor(LIGHTGRAY);
        return;
    }
    if (fileName.empty()) throw std::invalid_argument("Unable to access. File name is empty.");
    fileName = toLowerCase(fileName);
    if (fileName.length() < 4 || fileName.substr(fileName.length() - 4) != ".csv") fileName += ".csv";
    std::ofstream outFile(fileName, std::ios::out);
    if (!outFile) {
        std::cout << "Cannot open file. Please try again." << std::endl;
        return;
    }
    std::cout << getStartingIndex() << getEndingIndex() << std::endl;
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        Product product = products[i];
        outFile << stringifyProduct(product) << "\n";
    }
    setColor(GREEN);
    std::cout << "Successfully saved " << this->getSize() << " rows to " << fileName << "." << std::endl << std::endl;
    setColor(LIGHTGRAY);
    outFile.close();
}

void Database::load(std::string fileName) {
    if (this->getSize() != 0) {
        save(TEMPORARY_FILE_NAME);
        Database* temp = new Database();
        swapPointer(this, temp);
        delete temp;
    }
    // validate fileName
    if (fileName.empty()) throw std::invalid_argument("Unable to access. File name is empty.");
    fileName = toLowerCase(fileName);
    if (fileName.length() < 4 || fileName.substr(fileName.length() - 4) != ".csv") fileName += ".csv";
    std::ifstream inFile(fileName, std::ios::in);
    if (!inFile) {
        setColor(RED);
        std::cout << "File does not exist. Please try again." << std::endl;
        setColor(LIGHTGRAY);
        return;
    }
    std::string row;
    while (std::getline(inFile, row)) {
        if (row.empty()) continue;
        push_back(parseCSVRow(row));
    }

    system("cls");
    setColor(GREEN);
    std::cout << "Successfully loaded " << this->getSize() << " rows to the database." << std::endl << std::endl;
    setColor(LIGHTGRAY);
}

void Database:: ascendingSort() {
  for (int step = getStartingIndex(); step <= getEndingIndex(); step++) {
    std::string key = products[step].ID;
    int j = step - 1;

    // Compare key with each element on the left until an element smaller than it is found
    while (key < products[j].ID && j >= 0) {
      products[j + 1].ID = products[j].ID;
      j--;
    }
    products[j + 1].ID = key;
  }
}

void Database:: descendingSort(){
    for (int step = getStartingIndex(); step <= getEndingIndex(); step++) {
    std::string key = products[step].ID;
    int j = step - 1;

    // Compare key with each element on the left until an element bigger than it is found
    while (key > products[j].ID && j >= 0) {
      products[j + 1].ID = products[j].ID;
      j--;
    }
    products[j + 1].ID = key;
  }
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

// Display Menu
void displayModifyMenu() {
    int choice;
    // display the menu with 7 options for modifying product
    std::cout << "Please choose the following operations to modify the data:\n\t[1] CATEGORY\n\t[2] NAME\n\t[3] PRICE\n\t[4] QUANTITY\n\t[5] DESCRIPTION\n\t[6] ENTIRE ROW\n\t[0] EXIT\nYour choice: ";
}

Product Database::parseCSVRow(std::string row) {
    bool openQuote = false;
    int start = 0;
    int end = -1;
    std::string fields[6];
    int count = 0;
    for (int i = 0; i < row.length(); i++) {
        if (row[i] == '"') openQuote = !openQuote;
        if (!openQuote && (row[i] == ',' || i == row.length()-1)) {
            end = i;
            if (i == row.length() - 1) end++;
            fields[count++] = row.substr(start, end - start);
            start = end + 2;
        }
    }

    for (int i = 0; i < 6; i++) {
        if (fields[i][0] == '"') fields[i] = fields[i].substr(1);
        int length = fields[i].length();
        if (fields[i][length-1] == '"') fields[i] = fields[i].substr(0, length - 1);
    }

    std::string category = fields[0];
    std::string name = fields[1];
    double price = std::stod(fields[2]);
    std::string ID = fields[3];
    unsigned int quantity = std::stoi(fields[4]);
    std::string description = fields[5];

    Product product = {category, name, price, ID, quantity, description};
    return product;
}

std::string Database::stringifyProduct(Product product) {
    std::string s = "\"" + product.category + "\", " + "\"" + product.name + "\", " + std::to_string(product.price) + ", " + "\"" + product.ID + "\", " + std::to_string(product.quantity) + ", " +"\"" + product.description + "\"";

    return s;
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
void Database::printRow(const Product &product) {
    setColor(GREEN);
    printHeader();
    printProduct(product);
    setColor(LIGHTGRAY);
}

std::string limitStr(std::string s, int limit) {
    // if the length of the string exceeds the limit, then only leave the characters from index 0 to limit - 1, then add "..." to the string
    if (s.length() > limit) {
        s = s.substr(0, limit);
        s += "...";
    }
    return s;
}

std::string toLowerCase(std::string s) {
    std::string result;
    for (const char &c: s) {
        result += tolower(c);
    }
    return result;
}

void setColor(Color color) {
    // set the text in the concole to matcht the color
    SetConsoleTextAttribute(h, color);
}

void swapPointer(Database* a, Database* b) {
    Database* tmp = a;
    *a = *b;
    *b = *tmp;
}