#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
using namespace std;

// Menu printing
void printMenu(bool nextPrompt = false);

// Input validation helpers
bool isValidName(const string& s);
string getValidString();
int getValidInt();

#endif
