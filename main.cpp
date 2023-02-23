#include <iostream>
#include <limits>
#include <ctime>
#include <cmath>
#include "database.h"
#include <fstream>
using namespace std;

// general functions
void displayMenu();
string convertIdToString(unsigned long idNum);

// for formatting the inputs
string capitalize(string s);
string trim(string s);

int main() {
    srand(time(0));
    // clear screen when program starts
    system("cls");
    setColor(CYAN);
    cout << "<<<  Product Catalog Database  >>>" << endl << endl;
    bool exit = false;
    Database db;
    Product product;
    long id;
    string idStr;
    string fileName;
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
                setColor(WHITE);
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
                // clear screen, then print database
                system("cls");
                db.printDatabase();
                break;
            case 2:
                // if there are no more unique IDs left, end the program
                if (db.getSize() == pow(10, NUMBER_OF_DIGITS_FOR_ID) || NUMBER_OF_DIGITS_FOR_ID == 0) {
                    setColor(RED);
                    cout << "There are no more unique IDs! Please increase the number of digits for the ID" << endl;
                    setColor(WHITE);
                    exit = true;
                    break;
                }
                // clear screen, and then get information about the product, then push it to the database
                system("cls");
                product = db.getProductInfo();
                db.push_back(product);
                system("cls");
                setColor(GREEN);
                cout << "Item added to database" << endl;
                Database::printRow(product);
                setColor(WHITE);
                break;
            case 3:
                // if the database is not empty, delete first element, otherwise, display error message and continue
                db.pop_front();
                break;
            case 4:
                // if there is no product in database, display message and stop
                if (db.getSize() == 0) {
                    system("cls");
                    setColor(RED);
                    cout << "There are no products in the database" << endl << endl;
                    setColor(WHITE);
                    break;
                }

                while (true) {
                    // ask user for ID, then search it in database to modify it
                    cout << "\nPlease enter the ID of the product you want to modify: ";
                    // get ID from user, with validation
                    cin >> id;
                    if (cin.fail()) {
                        cout << "Please enter numeric values only for ID" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    if (id >= 0) break;
                    else {
                        cout << "Please enter positive numbers only.";
                    }
                }

                // convert numeric ID to string in its standard format
                idStr = convertIdToString(id);

                // run modify method, if there's no product, then the exception will be thrown
                try {
                    db.modify(idStr);
                }
                // catch the exception and print the message
                catch (invalid_argument e) {
                    setColor(LIGHTRED);
                    cout << e.what() << endl;
                    setColor(WHITE);
                }
                break;

            case 5:
                // if there's no product, display the message and stop
                if (db.getSize() == 0) {
                    system("cls");
                    setColor(RED);
                    cout << "There are no products in the database" << endl << endl;
                    setColor(WHITE);
                    break;
                }
                // get ID from user, while making sure that the ID is numeric values only
                while (true) {
                    cout << "\nPlease enter the ID of the item you want to search: ";
                    cin >> id;
                    if (cin.fail()) {
                        cout << "Please enter numeric values only for ID" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    if (id >= 0) break;
                    else {
                        cout << "Please enter positive numbers only.";
                    }
                }
                // convert numeric ID to string in its standard format
                idStr = convertIdToString(id);
                // search for ID, if ID doesn't exist, this will throw exception, if ID exists, this will print the product found
                try {
                    system("cls");
                    const Product& tmp = db.searchID(idStr);
                    Database::printRow(tmp);
                }
                // catch the exception and print the message
                catch (invalid_argument e) {
                    setColor(LIGHTRED);
                    cout << e.what() << endl;
                    setColor(WHITE);
                }
                break;

            case 6:
                // ascending sort
                db.ascendingSort();
                db.printDatabase();
                break;

            case 7:
                // descending sort
                db.descendingSort();
                db.printDatabase();
                break;

            case 8:
                // get file name from user to load the data
                cout << "\nEnter CSV file name: ";
                getline(cin, fileName);
                db.load(fileName);
                break;

            case 9:
                // if the database is empty, then we can't save
                if (db.getSize() == 0) {
                    system("cls");
                    setColor(RED);
                    cout << "The database is empty. Please add some data to save" << endl << endl;
                    setColor(WHITE);
                    break;
                }

                // get file name from user to save the data
                cout << "\nEnter CSV file name: ";
                getline(cin, fileName);
                db.save(fileName);
                break;

            default:
                // all other keys besides 0-4 are invalid, will prompt user to enter their choice again
                setColor(LIGHTRED);
                cout << "Invalid choice!" << endl;
                setColor(WHITE);
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
    cout << "Please choose the following operations:\n\t[1] SHOW TABLE\n\t[2] ADD PRODUCT\n\t[3] REMOVE PRODUCT\n\t[4] MODIFY PRODUCT\n\t[5] SEARCH PRODUCT\n\t[6] SORT DATABASE (ASCENDING)\n\t[7] SORT DATABASE (DESCENDING)\n\t[8] LOAD DATABASE\n\t[9] SAVE DATABASE\n\t[0] EXIT" << endl;
    cout << "Your choice: ";
}
