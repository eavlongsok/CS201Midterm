#include <iostream>
#include <cctype>
#include <iomanip>
#include <limits>
#include <Windows.h>
#include <stdlib.h>
#include <fstream>
#include "database.h"
#include <cmath>
#include <chrono>

// the line seperator
const std::string _SEPARATOR_ = "+" + std::string(141, '-') + "+";
// handle for setColor()
const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

// helper functions declaration
void printProduct(const Product &product);
void printHeader();

// to limit the length of string in the output
std::string limitStr(std::string s, int limit);
std::string capitalize(std::string s);
std::string trim(std::string s);
std::string convertIdToString(unsigned long idNum);
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
            tmp[j] = products[i];
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

// feature #1
void Database::printDatabase() {
    // if database is empty, return
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no products in the database" << std::endl << std::endl;
        setColor(WHITE);
        return;
    }
    // else print the header, then the rows
    setColor(MAGENTA);
    printHeader();
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        printProduct(this->products[i]);
    }
    setColor(WHITE);
}

// feature #1
void Database::printRow(const Product &product) {
    setColor(GREEN);
    printHeader();
    printProduct(product);
    setColor(WHITE);
}

// feature #2
void Database::push_back(Product product) {
    // if capacity reached, then reallocate to new memory block
    if (this->endingIndex + 1 == this->capacity) {
        reallocateProducts();
    }
    setSize(getSize() + 1);
    setEndingIndex();
    products[endingIndex] = product;
}

// feature #2
void Database::pop_front() {
    // if there's nothing in the database, return
    if (size == 0) {
        setColor(RED);
        std::cout << "There are no item in the database" << std::endl;
        setColor(WHITE);
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
    setColor(WHITE);
}

// feature #3
Product& Database::searchID(std::string id) {
    // search all products in database, if the ID match, then print that product
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        if (products[i].ID == id) {
            return products[i];
        }
    }
    // if not found, throw invalid_argument exception

    throw std::invalid_argument("Product Not Found");
}

// feature #4
void Database::modify(std::string id){
    // search for the ID in the database, if not found, it will throw exception, which we then catch in main function
    // if found, searchID(id) will return a reference to the product in the database, so that if any changes were made here, it will change the product in the array as well
    // by returning a reference, the searchID will return the actual product in the database(its location, similar to a pointer), and we can access the product in the database in here
    Product& product = searchID(id);
    bool exit = false;
    bool invalidInput = false;
    // always display menu, if the user make change to only 1 attribute, if the uesr chose to stop modify (by choosing 0), or the user modified the entire row, then we will stop this loop
    while (!exit) {
        if (!invalidInput) {
            system("cls");
            Database::printRow(product);
            displayModifyMenu();
        }
        invalidInput = false;
        // get the choice/operation from the user + validation
        int choice;
        while (true) {
            std::cin >> choice;
            if (std::cin.fail()) {
                setColor(LIGHTRED);
                std::cout << "Invalid choice!" << std::endl;
                setColor(WHITE);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Your choice: ";
                continue;
            }
            break;
        }

        switch (choice) {
            case 0:
                exit = true;
                system("cls");
                break;
            case 1:
                // modify category + validation
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                while (true) {
                    std::cout << "Enter the new Category: ";
                    getline(std::cin, product.category);
                    // remove spaces on both sides of the string
                    product.category = trim(product.category);
                    // if the trimmed string is not empty, continue, otherwise, prompt for the category again
                    if (!(product.category == "")) break;
                }
                product.category = capitalize(product.category);
                break;

            case 2:
                // modify name + validation
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                while (true) {
                    std::cout << "Enter the new Name: ";
                    getline(std::cin, product.name);
                    // remove spaces on both sides of the string
                    product.name = trim(product.name);
                    // if the trimmed string is not empty, continue, otherwise, prompt for the name again
                    if (!(product.name == "")) break;
                }
                product.name = capitalize(product.name);
                break;

            case 3:
                // modify price + validation
                while (true) {
                    std::cout << "Enter the new Price: ";
                    std::cin >> product.price;
                    // if the user input any characters that are not numeric, we prompt for input again
                    if (std::cin.fail()) {
                        std::cin.clear(); // clear the error flag
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    break;
                }
                // price is always positive
                product.price = abs(product.price);
                break;

            case 4:
                // modify quantity + validation
                while (true) {
                    std::cout << "Enter the new Quantity: ";
                    std::cin >> product.quantity;
                    // if the user input any characters that are not numeric, we prompt for input again
                    if (std::cin.fail()) {
                        std::cin.clear(); // chear the error flag
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    break;
                }
                // quantity is always positive
                product.quantity = abs(product.quantity);
                break;

            case 5:
                // modify description + validation
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter the new Description: ";
                getline(std::cin, product.description);
                // remove spaces on both sides of the string
                product.description = trim(product.description);
                // if the string is empty, modify the string to "No description"
                if (product.description == "") product.description = "No description";
                break;

            case 6:
                // modify entire row
                system("cls");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                product = getProductInfo(id);
                exit = true;
                break;

            default:
                // if the choice is invalid, display invalid choice
                setColor(LIGHTRED);
                std::cout << "Invalid choice!" << std::endl;
                setColor(WHITE);
                std::cout << "Your choice: ";
                // invalidInput here is used to make the display look nicer
                invalidInput = true;
                break;
        }
        if (!invalidInput)  Database::printRow(product);
    }
}

// feature #5
void Database::save(std::string fileName) {
    // if fileName is an empty string, throw an exception
    if (fileName.empty()) throw std::invalid_argument("Unable to open. File name is empty.");
    fileName = toLowerCase(fileName);

    // figure out if we should at .csv to the end of the string, since we support both input types ("name.csv" and just "name")
    if (fileName.length() <= 4 || fileName.substr(fileName.length() - 4) != ".csv") fileName += ".csv";

    // open the file, and check if we can open it
    std::ofstream outFile(fileName, std::ios::out);
    if (!outFile) {
        std::cout << "Cannot open file. Please try again." << std::endl;
        return;
    }

    // loop through all the products in the database, then convert them into string, then put them in the output csv file
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        Product product = products[i];
        outFile << stringifyProduct(product) << "\n";
    }

    system("cls");
    setColor(GREEN);
    std::cout << "Successfully saved " << this->getSize() << " rows to " << fileName << std::endl << std::endl;
    setColor(WHITE);
    outFile.close();
}

// feature #5
void Database::load(std::string fileName) {
    // if database has some information before loading, then the program saves it to a temporary file, and then load the new file
    if (this->getSize() != 0) {
        save(TEMPORARY_FILE_NAME);
        Database* temp = new Database();
        swapPointer(this, temp);
        delete temp;
    }

    // if fileName is an empty string, throw an exception
    if (fileName.empty()) throw std::invalid_argument("Unable to open. File name is empty.");
    fileName = toLowerCase(fileName);

    // figure out if we should at .csv to the end of the string, since we support both input types ("name.csv" and just "name")
    if (fileName.length() <= 4 || fileName.substr(fileName.length() - 4) != ".csv") fileName += ".csv";

    // open the file, and check if we can open it
    std::ifstream inFile(fileName, std::ios::in);
    if (!inFile) {
        system("cls");
        setColor(RED);
        std::cout << "File does not exist. Please try again." << std::endl;
        setColor(WHITE);
        return;
    }

    // get data from csv file row by row, as string initially, then we parse each row using parseCSVRow() function, then push back to database
    std::string row;
    while (std::getline(inFile, row)) {
        if (row.empty()) continue;
        push_back(parseCSVRow(row));
    }

    system("cls");
    setColor(GREEN);
    std::cout << "Successfully loaded " << this->getSize() << " rows from " << fileName << std::endl << std::endl;
    setColor(WHITE);
}

void Database:: ascendingSort() {
    auto now = std::chrono::system_clock::now();

    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        // currentProduct is the main product to compare to others
        Product currentProduct = products[i];
        std::string currentID = currentProduct.ID;
        // start comparing on the left side of ucurrentProduct
        int j = i - 1;

        // Compare currentID with each element on the left until an element with ID smaller than it is found
        while (currentID < products[j].ID && j >= 0) {
            // make the product on the right as the left one
            products[j + 1] = products[j];
            j--;
        }
        // make the products at the last index compared to be the currentProduct
        products[j + 1] = currentProduct;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - now;
    system("cls");
    std::cout << "Sorted " << this->getSize() << " items in " << elapsed_seconds.count() * 1000 << "ms\n";
}

void Database:: descendingSort(){
    auto now = std::chrono::system_clock::now();
    for (int i = getStartingIndex(); i <= getEndingIndex(); i++) {
        // currentProduct is the main product to compare to others
        Product currentProduct = products[i];
        std::string currentID = currentProduct.ID;
        // start comparing on the left side of ucurrentProduct
        int j = i - 1;

        // Compare currentID with each element on the left until an element with ID smaller than it is found
        while (currentID > products[j].ID && j >= 0) {
            products[j + 1] = products[j];
            j--;
        }
        // make the products at the last index compared to be the currentProduct
        products[j + 1] = currentProduct;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - now;
    system("cls");
    std::cout << "Sorted " << this->getSize() << " items in " << elapsed_seconds.count() * 1000 << "ms\n";
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
        tmp[j] = products[i];
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
    setColor(WHITE);
    int choice;
    // display the menu with 7 options for modifying product
    std::cout << "Please choose the following operations to modify the data:\n\t[1] CATEGORY\n\t[2] NAME\n\t[3] PRICE\n\t[4] QUANTITY\n\t[5] DESCRIPTION\n\t[6] ENTIRE ROW\n\t[0] EXIT\nYour choice: ";
}

Product Database::parseCSVRow(std::string row) {
    // check for quotes and commas
    bool openQuote = false;
    int start = 0;
    int end = -1;
    std::string fields[6];
    int count = 0;
    // loop throught the string "row"
    for (int i = 0; i < row.length(); i++) {
        // if string at index i is a double quote ("), then toggle the openQuote bool
        if (row[i] == '"') openQuote = !openQuote;
        // if there is no open quotes, and at index i, there's a comma or i is the last index
        if (!openQuote && (row[i] == ',' || i == row.length()-1)) {
            end = i;
            if (i == row.length() - 1) end++;
            // create a substring from the starting index, to the ending index, that substring is our data
            fields[count++] = row.substr(start, end - start);
            start = end + 2;
        }
    }

    // remove the double quote from the string
    for (int i = 0; i < 6; i++) {
        if (fields[i][0] == '"') fields[i] = fields[i].substr(1);
        int length = fields[i].length();
        if (fields[i][length-1] == '"') fields[i] = fields[i].substr(0, length - 1);
    }

    // assign each field of the product
    std::string category = fields[0];
    std::string name = fields[1];
    double price = std::stod(fields[2]);
    std::string ID = fields[3];
    int quantity = std::stoi(fields[4]);
    std::string description = fields[5];

    Product product = {category, name, price, ID, quantity, description};
    return product;
}

std::string Database::stringifyProduct(Product product) {
    // convert the product data to string, to be stored in csv file
    std::string s = "\"" + product.category + "\", " + "\"" + product.name + "\", " + std::to_string(product.price) + ", " + "\"" + product.ID + "\", " + std::to_string(product.quantity) + ", " +"\"" + product.description + "\"";

    return s;
}

Product Database::getProductInfo(std::string idStr) {
    // get all the information of the product
    std::string category, name, id, description;
    double price;
    int quantity;
    std::cout << "Please enter the product's information:" << std::endl;
    while (true) {
        std::cout << "Category: ";
        getline(std::cin, category);
        // remove spaces on both sides of the string
        category = trim(category);
        // if the trimmed string is not empty, continue, otherwise, prompt for the category again
        if (!(category == "")) break;
    }
    category = capitalize(category);
    while (true) {
        std::cout << "Name: ";
        getline(std::cin, name);
        // remove spaces on both sides of the string
        name = trim(name);
        // if the trimmed string is not empty, continue, otherwise, prompt for the name again
        if (!(name == "")) break;
    }
    name = capitalize(name);
    while (true) {
        std::cout << "Price: ";
        std::cin >> price;
        // if the user input any characters that are not numeric, we prompt for input again
        if (std::cin.fail()) {
            std::cin.clear(); // clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    price = abs(price);

    while (true) {
        std::cout << "Quantity: ";
        std::cin >> quantity;
        // if the user input any characters that are not numeric, we prompt for input again
        if (std::cin.fail()) {
            std::cin.clear(); // chear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    quantity = abs(quantity);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Description: ";
    getline(std::cin, description);
    // remove spaces on both sides of the string
    description = trim(description);
    // if the string is empty, modify the string to "No description"
    if (description == "") description = "No description";
    if (idStr.length() == 0) id = generateID();
    else id = idStr;
    Product tmp = {category, name, price, id, quantity, description};
    return tmp;
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

std::string limitStr(std::string s, int limit) {
    // if the length of the string exceeds the limit, then only leave the characters from index 0 to limit - 1, then add "..." to the string
    if (s.length() > limit) {
        s = s.substr(0, limit);
        s += "...";
    }
    return s;
}

std::string Database::generateID() {
    unsigned long idNum;
    std::string idStr;

    // loop to create a random ID, range for 0 to the 10^(num of digits), then convert it to string to match the standard format, if this new ID is unique, we return it, otherwise, loop until it finds a unique one
    do {
        idNum = rand() % (int)std::pow(10, NUMBER_OF_DIGITS_FOR_ID + 1);
        idStr = convertIdToString(idNum);
    } while (!this->uniqueID(idStr));
    return idStr;
}

bool Database::uniqueID(std::string idStr) {
    // search and compare the current id to all the id in the database
    for (int i = this->getStartingIndex(); i <= this->getEndingIndex(); i++) {
        if ((this->getProducts())[i].ID == idStr) return false;
    }
    return true;
}

std::string convertIdToString(unsigned long idNum) {
    std::string idStr = std::to_string(idNum);
    // add 0 to the front of the ID, until the number of digits match
    while (idStr.length() != NUMBER_OF_DIGITS_FOR_ID) {
        idStr = "0" + idStr;
    }
    return idStr;
}

std::string capitalize(std::string s) {
    std::string returnValue;
    // every character that is the first character, or has a space in front of it get capitalized
    for (int i = 0; i < s.length(); i++) {
        if (i == 0 || (s[i-1] == ' ' && isalpha(s[i])))
            returnValue += toupper(s[i]);
        else returnValue += s[i];
    }
    return returnValue;
}

// remove white space from the left and the right side of the string
std::string trim(std::string s) {
    int i = 0;
    // loop to find the index of first letter in the string
    while (s[i] == ' ') i++;
    int j = s.length() - 1;
    // loop to find the index of last letter in the string
    while (s[j] == ' ') j--;
    // return the trimmed string. (j-i+1 because that is the length of the actual string without white space on both sides)
    return s.substr(i, j-i+1);
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