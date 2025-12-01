#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

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

#endif

