// structure of database => struct Fields{...};
// container: called Database, act as vector, but has feature of stack
// features and specification: display data as row, has 3 data members, is an array, push_back, pop_front, search and display, search by ID

#include <iostream>
#include <vector>
#include "database.h"
using namespace std;

int main() {
    Database db;
    cout << "db:\n";
    db.printClassInfo();
    Product product1 = {"Phone", "IPhone", 1399.99, "69420", true, "A phone from Apple"};
    Product product2 = {"Laptop", "ZenBook", 1049.99, "1369", false, "A laptop from Asus"};
    Product product3 = {"Mouse", "Logitech G302", 42.99, "12903", true, "A wireless mouse from Logitech"};
    db.push_back(product1);
    db.push_back(product2);
    db.push_back(product3);
    db.printDatabase();

    // testing for copy constructor and assignment operator overloading
    // Database newDB;
    // newDB = db;
    // cout << "\nNewDB:\n";
    // newDB.printDatabase();
    // db.printClassInfo();

    // newDB.pop_front();
    // cout << "\ndb:\n";
    // db.printDatabase();
    // cout << "\nnewDB:\n";
    // newDB.printDatabase();

    cout << "Searching:" << endl;
    db.search(name, "iphone");
    db.search(category, "mouse");
    db.search(price, "10");
    db.searchID("69420");
}