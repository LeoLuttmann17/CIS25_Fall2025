# Week 13 - C++ Object-Oriented Programming: Product Class

## Overview
This project demonstrates fundamental Object-Oriented Programming (OOP) concepts in C++ by implementing a `Product` class with encapsulation, constructors, destructors, and member functions.

## Files

| File | Description |
|------|-------------|
| `Week13.cpp` | Main driver program |
| `Product.h` | Header file with class declaration |
| `Product.cpp` | Implementation file with method definitions |

## Class Structure

### Product Class

**Private Attributes:**
- `int id` — Product identifier
- `string name` — Product name
- `double price` — Product price

**Public Methods:**
- `Product(int i, string n, double p)` — Constructor
- `~Product()` — Destructor
- `getId()` — Returns product ID
- `getName()` — Returns product name
- `getPrice()` — Returns product price
- `setPrice(double p)` — Sets product price
- `printDetails()` — Displays product information

## Compilation

```bash
g++ -o Week13 Week13.cpp Product.cpp && ./Week13
```

## Sample Output

```
Howdy! Constructor called

Printing details below:
ID: 4567, Name: Ice Cream, Price: $2.99

Howdy! Destructor called
```

## Concepts Demonstrated

1. **Encapsulation** — Private data members with public accessors
2. **Constructor** — Initializes object with member initializer list
3. **Destructor** — Called automatically when object goes out of scope
4. **Getters/Setters** — Controlled access to private members
5. **Header Guards** — Separation of declaration (.h) and implementation (.cpp)

