#ifndef MANAGER_H_
#define MANAGER_H_

#include "Bank.hpp"
#include "BankAccount.hpp"

class Bank;
class BankAccount;
class User;
class Manager{


  std::string ID;
  std::string pin;
  std::vector<BankAccount*> accounts;
  std::vector<User*> users;

public:
  Manager(std::string ID, std::string pin, Bank *bank);
  std::string getManagerID();
  std::string getPin();
  bool validatePin(std::string pin);
  void addAccount(BankAccount *account);
  void addUser(User * user);
   //name of customer





};







#endif
