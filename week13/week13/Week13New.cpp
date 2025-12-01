#include <iostream>
#include <string>

// =======================
// Base Class: User
// =======================
class User {
public:
    User(const std::string& name) : username(name) {}
    virtual ~User() = default;

    virtual void accessLevel() const {
        std::cout << "Access level: BASIC USER — no permission to edit grades.\n";
    }

protected:
    std::string username;
};

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

// =======================
// Main Program
// =======================
int main() {
    InventoryManager hacker("Leo");

    std::cout << "== STUDENT TRANSCRIPT ACCESS TOOL ==\n";
    std::cout << "Running exploit...\n";
    std::cout << "Upgrading privileges for user: Leo...\n\n";

    hacker.accessLevel();   // Demonstrates polymorphism

    return 0;
}
