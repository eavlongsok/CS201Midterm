// structure of database => struct Fields{...};
// container: called Database, act as vector, but has feature of stack
// features and specification: display data as row, has 3 data members, is an array, push_back, pop_front, search and display, search by ID

#include <iostream>
#include <vector>
#include "database.h"
using namespace std;

void displayMenu();
Product getProductInfo();

int main() {
    setColor(CYAN);
    cout << "<<<  Product Catalog Database  >>>" << endl << endl;
    bool exit = false;
    Database db;
    Product product;
    while (!exit) {
        int choice;
        displayMenu();
        cin >> choice;
        switch (choice) {
            case 0:
                exit = true;
                break;
            case 1:
                product = getProductInfo();
                db.push_back(product);
                setColor(GREEN);
                cout << "Item added to database" << endl;
                setColor(BLACK);
                break;
            case 2:
                if (db.getSize() != 0) {
                    setColor(RED);
                    cout << "Deleted the first item in the database" << endl;
                    setColor(BLACK);
                }
                db.pop_front();
                break;
            case 3:
                db.printDatabase();
                break;
            default:
                setColor(LIGHTRED);
                cout << "Invalid choice!" << endl;
                setColor(BLACK);
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
    // setColor(BLACK);
}

void displayMenu() {
    setColor(WHITE);
    cout << "Please choose the following operations:\n\t[1] ADD PRODUCT\n\t[2] REMOVE PRODUCT\n\t[3] SHOW TABLE\n\t[0] EXIT" << endl;
    cout << "Your choice: ";
    setColor(BLACK);
}

Product getProductInfo() {
    Product tmp = {"Testing", "Testing", 10, "Testing", true, "Testing"};
    return tmp;
}