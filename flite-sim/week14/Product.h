#include <iostream>
#include <string>
using namespace std;

class Product {
// Explicitly private attributes
private:
    int id;
    string name;
    double price;
    
public:
    // Constructor
    Product(int i, string n, double p);
    
    // Destructor
    ~Product();
    
    // Getters
    int getId();
    string getName();
    double getPrice();
    
    // Setters
    void setPrice(double p);
    
    // Print details
    void printDetails();
};