#include <iostream>
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

const unsigned int NUMBER_OF_DIGITS_FOR_ID = 7; // will be used to generate random ID

int main() {
    // clear screen when program starts
    srand(time(0));
    system("cls");
    setColor(CYAN);
    cout << "<<<  Product Catalog Database  >>>" << endl << endl;
    bool exit = false;
    Database db;
    Product product;
    unsigned long id;
    string idStr;
    while (!exit) {
        // display the menu, and get choice from user
        int choice;
        displayMenu();
        // if the user input any characters that are not numeric, we prompt for input again
        while (true) {
            cin >> choice;
            if (cin.fail()) {
                setColor(LIGHTRED);
                cout << "Invalid choice!" << endl;
                setColor(LIGHTGRAY);
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Your choice: ";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        switch (choice) {
            case 0:
                exit = true;
                break;
            case 1:
                // if there are no more unique IDs left, end the program
                if (db.getSize() == pow(10, NUMBER_OF_DIGITS_FOR_ID) || NUMBER_OF_DIGITS_FOR_ID == 0) {
                    setColor(RED);
                    cout << "There are no more unique IDs! Please increase the number of digits for the ID" << endl;
                    setColor(LIGHTGRAY);
                    exit = true;
                    break;
                }
                // clear screen, and then get information about the product, then push it to the database
                system("cls");
                product = getProductInfo(db);
                db.push_back(product);
                setColor(GREEN);
                cout << "Item added to database" << endl;
                setColor(LIGHTGRAY);
                break;
            case 2:
                // if the database is not empty, delete first element, otherwise, display error message and continue
                db.pop_front();
                if (db.getSize() != 0) {
                    setColor(GREEN);
                    cout << "Deleted the first item in the database" << endl;
                    setColor(LIGHTGRAY);
                }
                break;
            case 3:
                // get ID from user, while making sure that the ID is numeric values only
                while (true) {
                    cout << "Please enter the ID of the item you want to search: ";
                    cin >> id;
                    if (cin.fail()) {
                        cout << "Please enter numeric values only for ID" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    break;
                }
                // convert numeric ID to string in its standard format
                idStr = convertIdToString(id);
                // search for ID
                db.searchID(idStr);
                break;
            case 4:
                // clear screen, then print database
                system("cls");
                db.printDatabase();
                break;
            default:
                // all other keys besides 0-4 are invalid, will prompty user to enter their choice again
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
    return 0;
}

void displayMenu() {
    // display the menu with 5 options
    setColor(WHITE);
    cout << "Please choose the following operations:\n\t[1] ADD PRODUCT\n\t[2] REMOVE PRODUCT\n\t[3] SEARCH ITEM\n\t[4] SHOW TABLE\n\t[0] EXIT" << endl;
    cout << "Your choice: ";
    setColor(LIGHTGRAY);
}

Product getProductInfo(const Database &db) {
    // get all the information of the product
    string category, name, id, description;
    double price;
    unsigned int quantity;
    cout << "Please enter the product's information:" << endl;
    while (true) {
        cout << "Category: ";
        getline(cin, category);
        // remove spaces on both sides of the string
        category = trim(category);
        // if the trimmed string is not empty, continue, otherwise, prompt for the category again
        if (!(category == "")) break;
    }
    category = capitalize(category);
    while (true) {
        cout << "Name: ";
        getline(cin, name);
        // remove spaces on both sides of the string
        name = trim(name);
        // if the trimmed string is not empty, continue, otherwise, prompt for the name again
        if (!(name == "")) break;
    }
    name = capitalize(name);
    while (true) {
        cout << "Price: ";
        cin >> price;
        // if the user input any characters that are not numeric, we prompt for input again
        if (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    price = abs(price);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    id = generateID(db);
    while (true) {
        cout << "Quantity: ";
        cin >> quantity;
        // if the user input any characters that are not numeric, we prompt for input again
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
    // remove spaces on both sides of the string
    description = trim(description);
    // if the string is empty, modify the string to "No description"
    if (description == "") description = "No description";
    Product tmp = {category, name, price, id, quantity, description};
    return tmp;
}

string generateID(const Database &db) {
    unsigned long idNum;
    string idStr;

    // loop to create a random ID, range for 0 to the 10^(num of digits), then convert it to string to match the standard format, if this new ID is unique, we return it, otherwise, loop until it finds a unique one
    do {
        idNum = rand() % (int)pow(10, NUMBER_OF_DIGITS_FOR_ID + 1);
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


