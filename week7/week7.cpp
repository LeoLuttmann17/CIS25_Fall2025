#include <iostream>
#include <string>
using namespace std;

// Define a struct named Item with two members: name (string) and id (int). 
struct Item { 
    string name; 
    int id; 
};

// Implement a linear search function to find an item by id. 
int linearSearch(Item* items, int size, int targetId) {
    for (int i = 0; i < size; ++i) {
        if (items[i].id == targetId) {
            return i; // Found
        }
    }
    return -1; // Not found
}

// Ask the user for an ID to search for, and display the result. 
int main() {
    // Use new to dynamically allocate an array of items. 
    Item* inventory = new Item[100];

    // Populate the array with realistic sample data
    string itemNames[] = {
        "Apple", "Banana", "Bread", "Butter", "Carrot", "Cheese", "Chicken", "Coffee",
        "Cookies", "Corn", "Cucumber", "Eggs", "Fish", "Flour", "Grapes", "Ham",
        "Ice Cream", "Juice", "Lettuce", "Milk", "Onion", "Orange", "Pasta", "Pepper",
        "Pizza", "Potato", "Rice", "Salad", "Salt", "Sandwich", "Soup", "Sugar",
        "Tea", "Tomato", "Water", "Yogurt", "Beef", "Cereal", "Chips", "Crackers",
        "Jam", "Ketchup", "Mayo", "Mustard", "Olives", "Pickles", "Sauce", "Spices",
        "Vinegar", "Wine", "Beer", "Soda", "Energy Drink", "Protein Bar", "Nuts",
        "Seeds", "Dried Fruit", "Candy", "Chocolate", "Cake", "Pie", "Donuts",
        "Muffins", "Bagels", "Crackers", "Popcorn", "Pretzels", "Trail Mix",
        "Granola", "Oatmeal", "Pancakes", "Waffles", "Syrup", "Honey", "Maple",
        "Vanilla", "Cinnamon", "Garlic", "Ginger", "Basil", "Oregano", "Thyme",
        "Rosemary", "Paprika", "Cumin", "Coriander", "Bay Leaves", "Peppercorns",
        "Chili Powder", "Red Pepper", "Black Pepper", "White Pepper", "Cardamom",
        "Cloves", "Nutmeg", "Allspice", "Turmeric", "Saffron", "Lemon", "Lime"
    };
    
    // Populate inventory with realistic data
    for (int i = 0; i < 100; ++i) {
        inventory[i].id = i + 1;
        inventory[i].name = itemNames[i];
    }
    
    // Sort the array by name (alphabetically)
    for (int i = 0; i < 99; ++i) {
        for (int j = 0; j < 99 - i; ++j) {
            if (inventory[j].name > inventory[j + 1].name) {
                // Swap items
                Item temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    } 

    int targetId;
    cout << "Enter the ID of the item to search for (1-100): ";
    
    // Check if input is valid
    if (!(cin >> targetId)) {
        cout << "Invalid input. Please enter a number between 1 and 100." << endl;
        cin.clear(); // Clear the error flag
        cin.ignore(10000, '\n'); // Ignore the invalid input
        delete[] inventory;
        return 0;
    }
    
    // Check if the ID is within valid range
    if (targetId < 1 || targetId > 100) {
        cout << "Invalid ID. Please enter a number between 1 and 100." << endl;
        delete[] inventory;
        return 0;
    }

    int resultIndex = linearSearch(inventory, 100, targetId);
    if (resultIndex != -1) {
        cout << "Item found: ID = " << inventory[resultIndex].id 
             << ", Name = " << inventory[resultIndex].name << endl;
    } else {
        cout << "Item with ID " << targetId << " not found." << endl;
    }

    // Free the memory using delete[]. 
    delete[] inventory;
    
    return 0;
}