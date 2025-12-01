#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "Employee.h"

// =======================
// Derived Class: InventoryManager (Hacker Upgrade Tool)
// =======================
class InventoryManager : public Employee {
public:
    InventoryManager(const std::string& name) : Employee(name) {}

    void accessLevel() const override {
        std::cout << "ACCESS GRANTED: ADMINISTRATOR MODE ENABLED\n";
        std::cout << "Yay, we hacked the system! \n";
        std::cout << "You can now change your Calculus grade from B+ to A-!\n";
    }
};

#endif

