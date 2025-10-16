#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <limits>

#include "strong_password.h"
#include "medium_password.h"
#include "weak_password.h"

using namespace std;

// Function to generate a strong password
string strongPassword() {
    string lowercase = "abcdefghijklmnopqrstuvwxyz";
    string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string numbers = "0123456789";
    string specials = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    string allChars = lowercase + uppercase + numbers + specials;

    string password;
    int specialCount = 0, numberCount = 0, upperCount = 0, lowerCount = 0;

    // Keep generating until conditions are met
    while (true) {
        password.clear();
        specialCount = numberCount = upperCount = lowerCount = 0;

        for (int i = 0; i < 8; i++) { // Increased length for better security
            char c = allChars[rand() % allChars.size()];
            password += c;
            if (isdigit(c)) numberCount++;
            if (specials.find(c) != string::npos) specialCount++;
            if (isupper(c)) upperCount++;
            if (islower(c)) lowerCount++;
        }

        // Ensure password has at least 2 numbers, 2 special chars, 1 uppercase, 1 lowercase
        if (specialCount >= 2 && numberCount >= 2 && upperCount >= 1 && lowerCount >= 1)
            break;
    }
    return password;
}

// Function to generate a medium password (no "years" input)
string mediumPassword() {
    string name;
    int birthYear;

    cout << "Enter your name: ";
    cin >> name;
    
    // Validate name input
    while (name.empty() || name.length() < 2) {
        cout << "Please enter a valid name (at least 2 characters): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> name;
    }
    
    cout << "Enter your year of birth: ";
    
    // Validate birth year input
    while (!(cin >> birthYear) || birthYear < 1900 || birthYear > 2025) {
        cout << "Please enter a valid birth year (1900-2025): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Combine name and birth year for password
    return name + to_string(birthYear);
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
    cout << "1. Strong (8+ chars, mixed case, numbers, special chars)\n";
    cout << "2. Medium (name + birth year)\n";
    cout << "3. Weak (simple password)\n";
    cout << "Enter your choice (1-3): ";

    int choice;
    
    // Validate choice input
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Invalid input. Please enter a number between 1 and 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

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
            return 1; // Return error code
    }

    return 0;
}
