#ifndef TRANSACTION_H_
#define TRANSACTION_H_
#include <ctime>
#include <string>
#include "BankAccount.hpp"
class BankAccount;
class Transaction {

  double amount;
  std::string amount1;
  time_t timeStamp;
  char *dt;
  std::string memo;
  BankAccount *inAccount;
  std::string account;
  std::string ID;

public:
  Transaction(double amount, BankAccount *inAccount);
  Transaction(double amount, std::string memo, BankAccount *inAccount);
  Transaction(std::string amount, std::string memo,
                           BankAccount *inAccount);
  Transaction(std::string userID, std::string account, std::string memo, double amount);
  double getAmount();
  std::string getSummaryLine();
  void writeToFile(std::string ID, std::string account, char * dt, std::string memo, double amount);


};

#endif
