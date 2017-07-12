#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include "BankAccount.hpp"
#include "Bank.hpp"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <iterator>
class Bank;
class BankAccount;
class User{


// std::string firstName;
// std::string lastName;
std::string name;
std::string ID;
std::string pin;
std::vector<BankAccount*> accounts;
time_t timeStamp;
char *dt;


public:
  // User(std::string firstName, std::string lastName, std::string pin, Bank* bank);
  User(std::string name, std::string pin, std::string ID);
  User(std::string name, std::string pin, Bank*bank);
  User(std::string ID);
  void addAccount(BankAccount *anAcct);
  std::string getID();
  bool validatePin(std::string pin);
  std::string getName();
  void printAccountsSummary();
  void printAccountsSummary(std::string ID);
  int numAccounts();
  std::string getPin();
  // std::string getAccountName();
  void printAcctTransHistory(int acctIdx);
  void printAllAcctsTransHistory();
  double getAcctBalance(int acctIdx);
  std::string getAcctID(int acctIdx);
  void addAcctTransactions(std::string acctIdx, double amount, std::string memo);
  bool accountExists(std::string name);//name of account
  void removeAccount(std::string name);
  // void setID(std::string ID); //haha w.e, cheating but who cares


};





#endif
