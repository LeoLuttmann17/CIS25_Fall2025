#include <iostream> 
using namespace std; 

int main() { 
	string itemName; 
	int quantity; 
	float cost; 
 
// Ask the user for required inputs 
	cout << "Enter item name: "; 
	cin >> itemName; 
	cout << "Enter quantity: "; 
	cin >> quantity; 
	cout << "Enter cost: "; 
	cin >> cost; 
 
	cout << "The total cost for " << itemName << " is $" << (quantity * cost) << endl; 
	return 0; 
} 
