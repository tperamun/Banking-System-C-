#ifndef BANK_H_
#define BANK_H_

#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include "User.hpp"
#include "Manager.hpp"
#include <fstream>
#include <iterator>
#include <vector>

class User;
class BankAccount;
class Manager;
class Bank{


  std::string name; //name of the bank
  std::vector<User*> users;
  std::vector<Manager*> managers;
  std::vector<BankAccount*> accounts;




public:
  Bank(std::string name);
  std::string getNewUserID();
  std::string getNewAccountID();
  void addAccount(BankAccount *anAcct);
  User *addUser(std::string name, std::string pin);
  void addUser(User *user);
  void addManager(Manager* manager);
  User* userLogin(std::string ID, std::string pin);
  Manager *managerLogin(std::string ID, std::string pin);
  Manager *addManager(std::string ID, std::string pin);
  std::string getName();
  User *giveUser(std::string ID);
  void printCustomerAccountDetails();

};





#endif
