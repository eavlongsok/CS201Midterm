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
string capitalize(string s);
string trim(string s);

const unsigned int NUMBER_OF_DIGITS_FOR_ID = 0; // at least 1

int main() {
    system("cls");
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
                if (db.getSize() == pow(10, NUMBER_OF_DIGITS_FOR_ID) || NUMBER_OF_DIGITS_FOR_ID == 0) {
                    setColor(RED);
                    cout << "All the IDs have been exhausted! Please increase the number of digits for the ID" << endl;
                    setColor(LIGHTGRAY);
                    exit = true;
                    break;
                }
                system("cls");
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
                system("cls");
                db.printDatabase();
                break;
            default:
                setColor(LIGHTRED);
                cout << "Invalid choice!" << endl;
                setColor(LIGHTGRAY);
                break;
        }
    }
    cout << "Exiting ";
    setColor(CYAN);
    cout << "<<<  Product Catalog Database  >>>" << endl;
    setColor(LIGHTGRAY);
}

void displayMenu() {
    setColor(WHITE);
    cout << "Please choose the following operations:\n\t[1] ADD PRODUCT\n\t[2] REMOVE PRODUCT\n\t[3] SEARCH ITEM\n\t[4] SHOW TABLE\n\t[0] EXIT" << endl;
    cout << "Your choice: ";
    setColor(LIGHTGRAY);
}

Product getProductInfo(const Database &db) {
    string category, name, id, description;
    double price;
    unsigned int quantity;
    cout << "Please enter the product's information:" << endl;
    while (true) {
        cout << "Category: ";
        getline(cin, category);
        category = trim(category);
        if (!(category == "")) break;
    }
    category = capitalize(category);
    while (true) {
        cout << "Name: ";
        getline(cin, name);
        name = trim(name);
        if (!(name == "")) break;
    }
    name = capitalize(name);
    while (true) {
        cout << "Price: ";
        cin >> price;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    price = abs(price);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // cout << "ID: ";
    // getline(cin, id);
    id = generateID(db);
    while (true) {
        cout << "Quantity: ";
        cin >> quantity;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Description: ";
    getline(cin, description);
    description = trim(description);
    if (description == "") description = "No description";
    Product tmp = {category, name, price, id, quantity, description};
    return tmp;
}

string generateID(const Database &db) {
    srand(time(0));
    unsigned long idNum;
    string idStr;

    // loop to create a random ID, range for 0 to the 10^(num of digits), then convert it to string to match the standard format, if this new ID is unique, we return it, otherwise, loop until it finds a unique one
    do {
    idNum = rand() % (int)pow(10, NUMBER_OF_DIGITS_FOR_ID);
    idStr = convertIdToString(idNum);
    } while (!uniqueID(idStr, db));
    return idStr;
}

bool uniqueID(string idStr, const Database &db) {
    // search and compare the current id to all the id in the database
    for (int i = db.getStartingIndex(); i <= db.getEndingIndex(); i++) {
        if ((db.getProducts())[i].ID == idStr) return false;
    }
    return true;
}

string convertIdToString(unsigned long idNum) {
    string idStr = to_string(idNum);
    // add 0 to the front of the ID, until the number of digits match
    while (idStr.length() != NUMBER_OF_DIGITS_FOR_ID) {
        idStr = "0" + idStr;
    }
    return idStr;
}

string capitalize(string s) {
    string returnValue;
    // every character that is the first character, or has a space in front of it get capitalized
    for (int i = 0; i < s.length(); i++) {
        if (i == 0 || (s[i-1] == ' ' && isalpha(s[i])))
            returnValue += toupper(s[i]);
        else returnValue += s[i];
    }
    return returnValue;
}

// remove white space from the left and the right side of the string
string trim(string s) {
    int i = 0;
    // loop to find the index of first letter in the string
    while (s[i] == ' ') i++;
    int j = s.length() - 1;
    // loop to find the index of last letter in the string
    while (s[j] == ' ') j--;
    // return the trimmed string. (j-i+1 because that is the length of the actual string without white space on both sides)
    return s.substr(i, j-i+1);
}


