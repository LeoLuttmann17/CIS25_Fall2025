#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

class Item {
public:
    string name;
    int quantity;

    Item(string n = "", int q = 0);
};

#endif
