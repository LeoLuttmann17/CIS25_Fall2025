#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Item {
public:
    string itemName;
    int quantity; 
    
    void saveToFile() { 
        ofstream out("items.txt", ios::app); // Append mode to add new lines
        if (out.is_open()) { 
            out << itemName << "," << quantity << endl; 
            out.close(); 
            cout << "Item " << itemName << " with quantity " << quantity << " saved to file." << endl; 
        } else { 
            cout << "Unable to open file for writing." << endl; 
        } 
    }

    void loadFromFile() { 
        ifstream in("items.txt"); 
        if (in.is_open()) { 
            string line; 
            while (getline(in, line)) { 
                cout << "File content: " << line << endl; 
            } 
            in.close(); 
        } else { 
            cout << "Unable to open file for reading." << endl; 
        } 
    } 
}; 

int main() {
    // Create and save the existing two items
    Item myItem;
    myItem.itemName = "Coffee";
    myItem.quantity = 50;
    myItem.saveToFile();
    
    Item myItem2;
    myItem2.itemName = "Tea";
    myItem2.quantity = 30;
    myItem2.saveToFile();
    
    cout << "Welcome to the Item Inventory System!" << endl;
    cout << "====================================" << endl;
    
    char choice;
    cout << "What would you like to do?" << endl;
    cout << "(A) Look up the quantity of an existing item" << endl;
    cout << "(B) Enter the name and quantity of a new item" << endl;
    cout << "Enter your choice (A or B): ";
    cin >> choice;
    
    Item inventory;
    
    if (choice == 'A' || choice == 'a') {
        inventory.loadFromFile();
        
    } else if (choice == 'B' || choice == 'b') {
        cout << "Enter item name: ";
        cin.ignore(); // Clear input buffer
        getline(cin, inventory.itemName);
        
        cout << "Enter item quantity: ";
        cin >> inventory.quantity;
        
        inventory.saveToFile();
        
    } else {
        cout << "Invalid choice. Please run the program again." << endl;
        return 1;
    }
    
    return 0;
}