// structure of database => struct Fields{...};
// container: called Database, act as vector, but has feature of stack
// features and specification: display data as row, has 3 data members, is an array, push_back, pop_front, search and display, search by ID

#include <iostream>
#include "database.h"
using namespace std;

int main() {
    Database db;
    cout << "db:\n";
    db.printClassInfo();
    Product product1 = {"Phone", "IPhone", 1299.99, "69420", true, "A phone from Apple"};
    Product product2 = {"Laptop", "ZenBook", 1049.99, "1369", false, "A laptop from Asus"};
    db.push_back(product1);
    db.push_back(product2);
    db.printDatabase();

    // testing for copy constructor and assignment operator overloading
    Database newDB;
    newDB = db;
    cout << "\nNewDB:\n";
    newDB.printDatabase();
}