x#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>
using namespace std;

class BankAccount {
private:
    double balance;
    void saveTransaction(const string& type, const string& item, double amount, double updatedBalance);

public:
    BankAccount();
    double getBalance() const;
    void deposit(double amount);
    void makePurchase(const string& item, double cost);
    void displayBalance() const;
    void showTransactionHistory();
};

#endif
