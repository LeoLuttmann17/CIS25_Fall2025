#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <limits>
#include "BankAccount.h"
#include "InputValidation.h"
#include "Menu.h"
using namespace std;

// ====================
// BankAccount Methods
// ====================
BankAccount::BankAccount() {
    balance = 1000.00; // Default starting balance
    ofstream file("transactions.txt");
    if (file.is_open()) {
        file << fixed << setprecision(2);
        file << "Starting Balance: $" << balance << endl;
        file.close();
    }
}

double BankAccount::getBalance() const {
    return balance;
}

void BankAccount::deposit(double amount) {
    balance += amount;
    cout << "Your new balance is: $" << fixed << setprecision(2) << balance << endl;
    saveTransaction("Deposit", "", amount, balance);
}

void BankAccount::makePurchase(const string& item, double cost) {
    if (cost > balance) {
        cout << "Ooops but you don't have enough money!" << endl;
    } else {
        balance -= cost;
        cout << "Your new balance is: $" << fixed << setprecision(2) << balance << endl;
        saveTransaction("Purchase", item, cost, balance);
    }
}

void BankAccount::displayBalance() const {
    cout << "Your current balance is: $" << fixed << setprecision(2) << balance << endl;
}

void BankAccount::showTransactionHistory() {
    ifstream file("transactions.txt");
    string line;
    bool hasTransactions = false;

    cout << "\nTransaction history:\n";
    cout << "--------------------------------------\n";

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("Deposit:") != string::npos || line.find("Purchase -") != string::npos) {
                hasTransactions = true;
            }
            cout << line << endl;
        }

        if (!hasTransactions) {
            cout << "No transactions yet. Try making a deposit or purchase first!\n";
        }

        file.close();
    } else {
        cout << "Unable to open transactions file.\n";
    }

    cout << "--------------------------------------\n";
}

void BankAccount::saveTransaction(const string& type, const string& item, double amount, double updatedBalance) {
    ofstream file("transactions.txt", ios::app);
    if (file.is_open()) {
        file << fixed << setprecision(2);
        if (type == "Deposit") {
            file << "Deposit: $" << amount
                 << " | Balance after transaction: $" << updatedBalance << endl;
        } else if (type == "Purchase") {
            file << "Purchase - " << item << ": $" << amount
                 << " | Balance after transaction: $" << updatedBalance << endl;
        }
        file.close();
    }
}

// ==========================
// Input Validation Functions
// ==========================
double getValidAmount(const string& prompt) {
    string input;
    double amount;
    regex validPattern(R"(^\d+(\.\d{1,2})?$)");

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (regex_match(input, validPattern)) {
            try {
                amount = stod(input);
                if (amount > 0) {
                    return amount;
                }
            } catch (...) {}
        }

        cout << "Oops enter a valid number:\n";
    }
}

string getValidItemName(const string& prompt) {
    string name;
    regex validPattern(R"(^[A-Za-z ]+$)");

    while (true) {
        cout << prompt;
        getline(cin, name);
        if (regex_match(name, validPattern)) {
            return name;
        }
        cout << "Oops enter a valid item name:\n";
    }
}

// =============
// Menu Function
// =============
void printMenu() {
    cout << "\n========= MENU =========\n";
    cout << "1. Deposit Money\n";
    cout << "2. Make a Payment\n";
    cout << "3. Check Balance\n";
    cout << "4. See Transaction History\n";
    cout << "5. Exit\n";
    cout << "========================\n";
    cout << "Choose an option: ";
}

// ====================
// Main Program
// ====================
int main() {
    BankAccount myAccount;
    int choice;

    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
    cout << "$$$ Hello, welcome to Leo's Bank Account Simulation! $$$\n";
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n";

    do {
        printMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid option (1-5).\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                double amount = getValidAmount("How much money do you want to deposit? ");
                myAccount.deposit(amount);
                break;
            }
            case 2: {
                string item = getValidItemName("What item are you purchasing? ");
                double cost = getValidAmount("How much does the item '" + item + "' cost? ");
                myAccount.makePurchase(item, cost);
                break;
            }
            case 3:
                myAccount.displayBalance();
                break;
            case 4:
                myAccount.showTransactionHistory();
                break;
            case 5:
                cout << "Bye, bye see you next time!\n";
                break;
            default:
                cout << "Please choose a valid option (1-5).\n";
                break;
        }

    } while (choice != 5);

    return 0;
}
