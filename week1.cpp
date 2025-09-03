// main.cpp
#include <iostream>
using namespace std;
 
int main() {
    // Print greeting message with escape characters
    cout << "Hello Inventory System!\n";
    cout << "Welcome to the Inventory Program.\n\n";
 
    // Demonstrating escape characters
    cout << "Here are some escape characters in action:\n";
    cout << "\t- Tabbed line\n";
    cout << "This is a backslash: \\\n";
    cout << "This is a double quote: \"Hello!\"\n\n";
 
    // Output sizes of basic data types
    cout << "Data Type Sizes:\n";
    cout << "\tSize of int: " << sizeof(int) << " bytes\n";
    cout << "\tSize of float: " << sizeof(float) << " bytes\n";
    cout << "\tSize of char: " << sizeof(char) << " byte\n";
 
    return 0;
}
