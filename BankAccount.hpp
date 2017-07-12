#ifndef BANKACCOUNT_H_
#define BANKACCOUNT_H_

#include <vector>
#include <string>
#include "Transaction.hpp"
#include "Bank.hpp"
#include "User.hpp"
#include <iomanip>
#include <sstream>
class Transaction;
class Bank;
class User;
class BankAccount{

  std::string name;
  // double balance; //dont need it, ill calculate it from the transactions
  std::string ID; //account ID
  User *accountHolder;
  std::vector<Transaction*> transactions;

public:
  BankAccount(std::string name, User *holder, Bank *bank);
  std::string getID();
  std::string getSummaryLine();
  std::string getBankAccountName();
  double getBalance();
  std::string getAccountHolderName();
  std::string getAccountHolderPin();
  User* getHolder();
  void printTransHistory();
  void addTransaction(double amount, std::string memo);
};






#endif
