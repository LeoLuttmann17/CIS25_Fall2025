#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

// Function to generate a strong password
string strongPassword() {
    string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string numbers = "0123456789";
    string specials = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    string allChars = letters + numbers + specials;

    string password;
    int specialCount = 0, numberCount = 0;

    // Keep generating until conditions are met
    while (true) {
        password.clear();
        specialCount = numberCount = 0;

        for (int i = 0; i < 7; i++) {
            char c = allChars[rand() % allChars.size()];
            password += c;
            if (isdigit(c)) numberCount++;
            if (specials.find(c) != string::npos) specialCount++;
        }

        if (specialCount >= 2 && numberCount >= 2)
            break;
    }
    return password;
}

// Function to generate a medium password
string mediumPassword() {
    string name;
    int birthYear;
    int years;

    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your year of birth: ";
    cin >> birthYear;
    cout << "Enter how many years (e.g., since something important): ";
    cin >> years;

    return name + to_string(years) + to_string(birthYear);
}

// Function to generate a weak password
string weakPassword() {
    return "password";
}

int main() {
    srand(time(0)); // Seed for randomness

    cout << "How are you?\n";
    cout << "Welcome to Leo's Password Maker!\n";
    cout << "How strong do you want your password to be?\n";
    cout << "1. Strong\n2. Medium\n3. Weak\n";
    cout << "Enter your choice (1-3): ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Your strong password is: " << strongPassword() << endl;
            break;
        case 2:
            cout << "Your medium password is: " << mediumPassword() << endl;
            break;
        case 3:
            cout << "Your weak password is: " << weakPassword() << endl;
            break;
        default:
            cout << "Invalid choice. Please run the program again." << endl;
    }

    return 0;
}
