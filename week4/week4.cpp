#include <iostream>
#include <thread>   // for sleep_for
#include <chrono>   // for seconds
using namespace std;

// Delivery helper function
void deliverOrder() {
    cout << "Your order will be delivered shortly!" << endl;
    this_thread::sleep_for(chrono::seconds(10));  // wait for 10 seconds
    cout << "Your order is here!" << endl;
}

int main() {
    int choice;

    cout << "What do you want to do?\n";
    cout << "(1) Eat\n";
    cout << "(2) Drink\n";
    cout << "(3) Snack\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) { // Eat
        char foodChoice;
        cout << "What do you want to eat?\n";
        cout << "(A) Pizza\n";
        cout << "(B) Sushi\n";
        cout << "(C) Pani Puri\n";
        cout << "Enter your choice: ";
        cin >> foodChoice;

        if (foodChoice == 'A' || foodChoice == 'a') {
            char pizzaType;
            cout << "You chose Pizza!\n";
            cout << "Choose pizza type:\n";
            cout << "(R) Regular\n";
            cout << "(G) Gluten-free\n";
            cout << "Enter your choice: ";
            cin >> pizzaType;

            if (pizzaType == 'R' || pizzaType == 'r')
                cout << "Thank you for ordering Regular Pizza!" << endl;
            else if (pizzaType == 'G' || pizzaType == 'g')
                cout << "Thank you for ordering Gluten-free Pizza!" << endl;
            else {
                cout << "Invalid pizza type choice." << endl;
                return 0;
            }
            deliverOrder();
        }
        else if (foodChoice == 'B' || foodChoice == 'b') {
            char sushiType;
            cout << "You chose Sushi!\n";
            cout << "Choose sushi type:\n";
            cout << "(T) Tuna\n";
            cout << "(S) Salmon\n";
            cout << "(V) Vegetarian\n";
            cout << "Enter your choice: ";
            cin >> sushiType;

            if (sushiType == 'T' || sushiType == 't')
                cout << "Thank you for ordering Tuna Sushi!" << endl;
            else if (sushiType == 'S' || sushiType == 's')
                cout << "Thank you for ordering Salmon Sushi!" << endl;
            else if (sushiType == 'V' || sushiType == 'v')
                cout << "Thank you for ordering Vegetarian Sushi!" << endl;
            else {
                cout << "Invalid sushi type choice." << endl;
                return 0;
            }
            deliverOrder();
        }
        else if (foodChoice == 'C' || foodChoice == 'c') {
            char spiceLevel;
            cout << "You chose Pani Puri!\n";
            cout << "Choose spice level:\n";
            cout << "(S) Spicy\n";
            cout << "(M) Medium Spicy\n";
            cout << "(V) Very Spicy\n";
            cout << "Enter your choice: ";
            cin >> spiceLevel;

            if (spiceLevel == 'S' || spiceLevel == 's')
                cout << "Thank you for ordering Spicy Pani Puri!" << endl;
            else if (spiceLevel == 'M' || spiceLevel == 'm')
                cout << "Thank you for ordering Medium Spicy Pani Puri!" << endl;
            else if (spiceLevel == 'V' || spiceLevel == 'v')
                cout << "Thank you for ordering Very Spicy Pani Puri!" << endl;
            else {
                cout << "Invalid spice level choice." << endl;
                return 0;
            }
            deliverOrder();
        }
        else {
            cout << "Invalid food choice." << endl;
        }
    }
    else if (choice == 2) { // Drink
        char drinkChoice;
        cout << "You chose to Drink!\n";
        cout << "Choose a drink:\n";
        cout << "(M) Mango Lassi\n";
        cout << "(S) Strawberry Juice\n";
        cout << "(C) Coffee\n";
        cout << "Enter your choice: ";
        cin >> drinkChoice;

        if (drinkChoice == 'M' || drinkChoice == 'm') {
            int sweetness;
            cout << "You chose Mango Lassi!\n";
            cout << "Choose sweetness level:\n";
            cout << "1. Sweet\n";
            cout << "2. Medium Sweet\n";
            cout << "3. Ultra Sweet\n";
            cout << "Enter your choice: ";
            cin >> sweetness;

            if (sweetness == 1)
                cout << "Thank you for ordering Sweet Mango Lassi!" << endl;
            else if (sweetness == 2)
                cout << "Thank you for ordering Medium Sweet Mango Lassi!" << endl;
            else if (sweetness == 3)
                cout << "Thank you for ordering Ultra Sweet Mango Lassi!" << endl;
            else {
                cout << "Invalid sweetness choice." << endl;
                return 0;
            }
            deliverOrder();
        }
        else if (drinkChoice == 'S' || drinkChoice == 's') {
            char chunksChoice;
            cout << "You chose Strawberry Juice!\n";
            cout << "Would you like strawberry chunks in it? (Y/N): ";
            cin >> chunksChoice;

            if (chunksChoice == 'Y' || chunksChoice == 'y')
                cout << "Thank you for ordering Strawberry Juice with chunks!" << endl;
            else if (chunksChoice == 'N' || chunksChoice == 'n')
                cout << "Thank you for ordering Strawberry Juice without chunks!" << endl;
            else {
                cout << "Invalid choice for strawberry chunks." << endl;
                return 0;
            }
            deliverOrder();
        }
        else if (drinkChoice == 'C' || drinkChoice == 'c') {
            char coffeeSugar;
            cout << "You chose Coffee!\n";
            cout << "Do you want sugar? (Y/N): ";
            cin >> coffeeSugar;

            if (coffeeSugar == 'Y' || coffeeSugar == 'y')
                cout << "Thank you for ordering Coffee with Sugar!" << endl;
            else if (coffeeSugar == 'N' || coffeeSugar == 'n')
                cout << "Thank you for ordering Coffee without Sugar!" << endl;
            else {
                cout << "Invalid choice for coffee sugar option." << endl;
                return 0;
            }
            deliverOrder();
        }
        else {
            cout << "Invalid drink choice." << endl;
        }
    }
    else if (choice == 3) { // Snack
        char snackChoice;
        cout << "You chose to Snack!\n";
        cout << "Choose a snack:\n";
        cout << "(V) Veggie Sticks\n";
        cout << "(D) Doritos\n";
        cout << "(C) Cheetos\n";
        cout << "Enter your choice: ";
        cin >> snackChoice;

        if (snackChoice == 'V' || snackChoice == 'v') {
            cout << "Thank you for ordering Veggie Sticks!" << endl;
            deliverOrder();
        }
        else if (snackChoice == 'D' || snackChoice == 'd') {
            char doritosFlavor;
            cout << "You chose Doritos!\n";
            cout << "Choose Doritos flavor:\n";
            cout << "(S) Spicy\n";
            cout << "(A) Sea Salt\n";
            cout << "(N) None\n";
            cout << "Enter your choice: ";
            cin >> doritosFlavor;

            if (doritosFlavor == 'S' || doritosFlavor == 's')
                cout << "Thank you for ordering Spicy Doritos!" << endl;
            else if (doritosFlavor == 'A' || doritosFlavor == 'a')
                cout << "Thank you for ordering Sea Salt Doritos!" << endl;
            else if (doritosFlavor == 'N' || doritosFlavor == 'n')
                cout << "Thank you for ordering Plain Doritos!" << endl;
            else {
                cout << "Invalid Doritos flavor choice." << endl;
                return 0;
            }
            deliverOrder();
        }
        else if (snackChoice == 'C' || snackChoice == 'c') {
            char cheetosType;
            cout << "You chose Cheetos!\n";
            cout << "Choose Cheetos type:\n";
            cout << "(U) Ultra Spicy\n";
            cout << "(S) Spicy\n";
            cout << "(C) Crunchy\n";
            cout << "Enter your choice: ";
            cin >> cheetosType;

            if (cheetosType == 'U' || cheetosType == 'u')
                cout << "Thank you for ordering Ultra Spicy Cheetos!" << endl;
            else if (cheetosType == 'S' || cheetosType == 's')
                cout << "Thank you for ordering Spicy Cheetos!" << endl;
            else if (cheetosType == 'C' || cheetosType == 'c')
                cout << "Thank you for ordering Crunchy Cheetos!" << endl;
            else {
                cout << "Invalid Cheetos type choice." << endl;
                return 0;
            }
            deliverOrder();
        }
        else {
            cout << "Invalid snack choice." << endl;
        }
    }
    else {
        cout << "Invalid choice." << endl;
    }

    return 0;
}
