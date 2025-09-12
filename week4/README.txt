# Food Ordering Simulator
 
A simple **C++ console program** that simulates a food ordering system. Users can choose to **Eat**, **Drink**, or **Snack**, pick from different options, and watch their "order" be delivered after a short wait.
 
---
 
## 📋 Features

- Menu-driven interface for:

  - **Eat**: Pizza, Sushi, Pani Puri (with further customization like type or spice level).

  - **Drink**: Mango Lassi, Strawberry Juice, Coffee (with customization like sweetness or sugar).

  - **Snack**: Veggie Sticks, Doritos, or Cheetos (with flavor/type choices).

- **Order delivery simulation** using `std::this_thread::sleep_for` (10-second wait).

- Input validation for invalid choices.

- Clear, user-friendly prompts.
 
---
 
## 🛠 Requirements

- **C++11 or newer** (for `<thread>` and `<chrono>` support).

- A terminal or command prompt.
 
---
 
## ▶️ How to Compile and Run
 
### **Linux/macOS**

```bash

g++ -std=c++11 food_ordering.cpp -o food_ordering

./food_ordering

 