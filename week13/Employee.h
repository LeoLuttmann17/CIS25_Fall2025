#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "User.h"

// =======================
// Derived Class: Employee
// =======================
class Employee : public User {
public:
    Employee(const std::string& name) : User(name) {}

    void accessLevel() const override {
        std::cout << "Access level: EMPLOYEE — can view student transcripts.\n";
    }
};

#endif

