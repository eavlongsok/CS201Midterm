// structure of database => struct Fields{...};
// container: called Database, act as vector, but has feature of stack
// features and specification: display data as row, has 3 data members, is an array, push_back, pop_front, search and display, search by ID

#include <iostream>
#include <vector>
#include <limits>
#include <ctime>
#include <cmath>
#include "database.h"
using namespace std;

void displayMenu();
Product getProductInfo(const Database &db);
string generateID(const Database &db);
bool uniqueID(string idStr, const Database &db);
string convertIdToString(unsigned long idNum);

const unsigned int NUMBER_OF_DIGITS_FOR_ID = 5;

int main() {
    setColor(CYAN);
    cout << "<<<  Product Catalog Database  >>>" << endl << endl;
    bool exit = false;
    Database db;
    Product product;
    unsigned long id;
    string idStr;
    while (!exit) {
        int choice;
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 0:
                exit = true;
                break;
            case 1:
                product = getProductInfo(db);
                db.push_back(product);
                setColor(GREEN);
                cout << "Item added to database" << endl;
                setColor(LIGHTGRAY);
                break;
            case 2:
                if (db.getSize() != 0) {
                    setColor(RED);
                    cout << "Deleted the first item in the database" << endl;
                    setColor(LIGHTGRAY);
                }
                db.pop_front();
                break;
            case 3:
                cout << "Please enter the ID of the item you want to search: ";
                cin >> id;
                idStr = convertIdToString(id);
                db.searchID(idStr);
                break;
            case 4:
                db.printDatabase();
                break;
            default:
                setColor(LIGHTRED);
                cout << "Invalid choice!" << endl;
                setColor(LIGHTGRAY);
                break;
        }
    }
    // Database db;
    // db.printDatabase();
    // cout << "db:\n";
    // db.printClassInfo();
    // Product product1 = {"Phone", "IPhone", 1399.99, "69420", true, "A phone from Apple"};
    // Product product2 = {"Laptop", "ZenBook", 1049.99, "1369", false, "A laptop from Asus"};
    // Product product3 = {"Mouse", "Logitech G302", 42.99, "12903", true, "A wireless mouse from Logitech"};
    // db.push_back(product1);
    // db.push_back(product2);
    // db.push_back(product3);
    // db.printDatabase();

    // db.printClassInfo();
    // db.pop_front();
    // db.printClassInfo();
    // db.printDatabase();

    // // testing for copy constructor and assignment operator overloading
    // Database newDB;
    // newDB = db;
    // cout << "\nAfter NewDB is copied:\n";
    // newDB.printDatabase();
    // newDB.printClassInfo();

    // cout << "After newDB popped front:\n";
    // newDB.pop_front();
    // cout << "\ndb:\n";
    // db.printDatabase();
    // cout << "\nnewDB:\n";
    // newDB.printDatabase();
    // newDB.printClassInfo();
    // cout << "Searching:" << endl;

    // db.searchID("1");
    // setColor(LIGHTGRAY);
}

void displayMenu() {
    setColor(WHITE);
    cout << "Please choose the following operations:\n\t[1] ADD PRODUCT\n\t[2] REMOVE PRODUCT\n\t[3] SEARCH ITEM\n\t[4] SHOW TABLE\n\t[0] EXIT" << endl;
    cout << "Your choice: ";
    setColor(LIGHTGRAY);
}

Product getProductInfo(const Database &db) {
    // Product tmp = {"Testing", "Testing", 10, "Testing", true, "Testing"};
    string category, name, id, description;
    double price;
    bool status;
    char tmpStatus;
    cout << "Please enter the product's information:" << endl;
    cout << "Category: ";
    getline(cin, category);
    cout << "Name: ";
    getline(cin, name);
    cout << "Price: ";
    cin >> price;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // cout << "ID: ";
    // getline(cin, id);
    id = generateID(db);
    cout << "In stock?(y/n): ";
    cin >> tmpStatus;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (tmpStatus == 'y' || tmpStatus == 'Y') status = true;
    else status = false;
    cout << "Description: ";
    getline(cin, description);
    if (description == "") description = "No description";
    Product tmp = {category, name, price, id, status, description};
    return tmp;
}

string generateID(const Database &db) {
    srand(time(0));
    unsigned long idNum;
    string idStr;
    do {
    idNum = rand() % (int)pow(10, NUMBER_OF_DIGITS_FOR_ID);
    idStr = convertIdToString(idNum);
    } while (!uniqueID(idStr, db));
    return idStr;
}

bool uniqueID(string idStr, const Database &db) {

    for (int i = db.getStartingIndex(); i <= db.getEndingIndex(); i++) {
        if ((db.getProducts())[i].ID == idStr) return false;
    }
    return true;
}

string convertIdToString(unsigned long idNum) {
    string idStr = to_string(idNum);
    while (idStr.length() != NUMBER_OF_DIGITS_FOR_ID) {
        idStr = "0" + idStr;
    }
    return idStr;
}