#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include "Item.h"
#include "functions.h"
using namespace std;

// ===============================
// Item constructor
// ===============================
Item::Item(string n, int q) {
    name = n;
    quantity = q;
}

// ===============================
// Function implementations
// ===============================
void printMenu(bool nextPrompt) {
    if (nextPrompt)
        cout << "\nWhat do you want to do next?\n";
    else
        cout << "\n=== Leo's Inventory Program ===\n";

    cout << "1. View existing inventory\n";
    cout << "2. Enter new item names and quantities\n";
    cout << "3. Search for item by name\n";
    cout << "0. Exit program\n";
    cout << "Enter your choice (0–3): ";
}

bool isValidName(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isalnum(c) && !isspace(c))
            return false;
    }
    return true;
}

string getValidString() {
    string input;
    while (true) {
        cin >> ws;
        getline(cin, input);
        if (isValidName(input))
            return input;
        cout << "Invalid name. Please use only letters, numbers, and spaces: ";
    }
}

int getValidInt() {
    string input;
    int value;
    while (true) {
        cin >> ws;
        getline(cin, input);

        bool allDigits = !input.empty() && all_of(input.begin(), input.end(), ::isdigit);

        if (allDigits) {
            value = stoi(input);
            if (value >= 0)
                return value;
        }
        cout << "Invalid quantity. Please enter a positive number: ";
    }
}

// ===============================
// Main program
// ===============================
int main() {
    cout << "Hi, welcome to Leo's Inventory Program!" << endl;

    Item inventory[5] = {
        Item("Espresso", 10),
        Item("Latte", 7),
        Item("Cappuccino", 5),
        Item("Mocha", 8),
        Item("Americano", 12)
    };

    int choice;
    printMenu();

    while (true) {
        if (!(cin >> choice)) {
            cout << "Ooops, please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printMenu(true);
            continue;
        }

        if (choice == 0) {
            cout << "Goodbye! Thanks for using Leo's Inventory Program.\n";
            break;
        }

        else if (choice == 1) {
            cout << "\n--- Existing Inventory ---\n";
            for (int i = 0; i < 5; i++)
                cout << inventory[i].name << " - Quantity: " << inventory[i].quantity << endl;
        }

        else if (choice == 2) {
            cout << "\n--- Enter New Items ---\n";
            for (int i = 0; i < 5; i++) {
                cout << "Enter item name #" << (i + 1) << ": ";
                inventory[i].name = getValidString();

                cout << "Enter item quantity #" << (i + 1) << ": ";
                inventory[i].quantity = getValidInt();
            }
            cout << "\nThank you, the inventory has been updated.\n";
        }

        else if (choice == 3) {
            string searchName;
            bool found = false;

            while (true) {
                cout << "Enter item name to search (or type QUIT to exit): ";
                searchName = getValidString();

                if (searchName == "QUIT" || searchName == "quit") {
                    cout << "Returning to main menu...\n";
                    break;
                }

                found = false;
                for (int i = 0; i < 5; i++) {
                    if (inventory[i].name == searchName) {
                        cout << "Found " << searchName
                             << " - Quantity: " << inventory[i].quantity << endl;
                        found = true;
                        break;
                    }
                }

                if (!found)
                    cout << "Oops, I can't find this. Please try again or type QUIT to exit.\n";
                else
                    break;
            }
        }

        else {
            cout << "Ooops, please try again.\n";
        }

        printMenu(true);
    }

    return 0;
}
