#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <iostream>
#include <fstream>
#include "utils.h"

// Handles login/signup
// Returns false if user exits
inline bool authenticateUser(std::string& currentUser,
                             std::string& favoritesFile) {
    while (true) {
        std::cout << "Welcome! Type 1 to Login, 2 to Sign Up, or 0 to Exit: ";
        std::string option;
        getline(std::cin, option);
        option = trim(option);

        if (option == "0") return false;

        if (option == "2") {  // Sign up
            std::cout << "Sign up - enter your username: ";
            std::string name;
            getline(std::cin, name);
            name = trim(name);

            std::cout << "Enter your password: ";
            std::string password;
            getline(std::cin, password);
            password = trim(password);

            std::ifstream userCheck("users.csv");
            std::string uline;
            while (getline(userCheck, uline)) {
                auto f = splitCSVLine(uline);
                if (f.size() >= 2 && f[0] == name) {
                    std::cout << "That username is already taken.\n";
                    goto retry;
                }
            }

            {
                std::ofstream newUser("users.csv", std::ios::app);
                newUser << name << "," << password << "\n";
            }
            std::cout << "Sign up successful! Please log in.\n";
        }
        else if (option == "1") {  // Login
            std::cout << "Login - enter your username: ";
            std::string name;
            getline(std::cin, name);
            name = trim(name);

            std::cout << "Enter your password: ";
            std::string password;
            getline(std::cin, password);
            password = trim(password);

            std::ifstream userFile("users.csv");
            std::string uline;
            while (getline(userFile, uline)) {
                auto f = splitCSVLine(uline);
                if (f.size() >= 2 && f[0] == name && f[1] == password) {
                    currentUser = name;
                    favoritesFile = "favorites_" + name + ".csv";
                    std::cout << "Welcome, " << name << "!\n";
                    return true;
                }
            }
            std::cout << "Incorrect username or password.\n";
        }
        else {
            std::cout << "Invalid option.\n";
        }

    retry:;
    }
}

#endif
