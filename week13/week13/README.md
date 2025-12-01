# Week 13: Inheritance and Polymorphism Demo

## Description

This program demonstrates object-oriented programming concepts in C++, specifically **inheritance** and **polymorphism**. It simulates a student transcript access system with different user privilege levels.

## Program Overview

The program creates a class hierarchy to model different types of users in a system:
- **User** (Base Class): Basic user with limited permissions
- **Employee** (Derived from User): Employee with standard permissions
- **InventoryManager** (Derived from Employee): Administrator with full access

## Class Hierarchy

```
User (Base Class)
  └── Employee (Derived Class)
       └── InventoryManager (Derived Class)
```

## Key Concepts Demonstrated

1. **Inheritance**: Classes inherit from parent classes using the `public` keyword
2. **Polymorphism**: Virtual functions allow derived classes to override base class behavior
3. **Virtual Functions**: The `accessLevel()` method is declared as `virtual` in the base class
4. **Method Overriding**: Derived classes use `override` to provide their own implementation
5. **Protected Members**: The `username` member is protected, allowing derived classes to access it

## Class Details

### User (Base Class)
- **Constructor**: Takes a name and initializes the username
- **Virtual Destructor**: Ensures proper cleanup of derived objects
- **accessLevel()**: Virtual function that displays basic user permissions

### Employee (Derived from User)
- **Constructor**: Calls the User constructor to initialize the base class
- **accessLevel()**: Overrides the base class method to show employee-level permissions

### InventoryManager (Derived from Employee)
- **Constructor**: Calls the Employee constructor
- **accessLevel()**: Overrides to show administrator-level access with full privileges

## How to Compile

```bash
g++ -o Week13New Week13New.cpp
```

## How to Run

```bash
./Week13New
```

## Expected Output

```
== STUDENT TRANSCRIPT ACCESS TOOL ==
Running exploit...
Upgrading privileges for user: Leo...

ACCESS GRANTED: ADMINISTRATOR MODE ENABLED
Yay, we hacked the system! 
You can now change your Calculus grade from B+ to A-!
```

## Code Structure

- All classes are defined in a single file (`Week13New.cpp`)
- The program uses the standard C++ libraries: `<iostream>` and `<string>`
- The main function creates an `InventoryManager` object and demonstrates polymorphism by calling the overridden `accessLevel()` method

## Author

Leo

## Notes

This program is a demonstration of C++ OOP concepts and is intended for educational purposes.

