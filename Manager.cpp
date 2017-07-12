#include "Manager.hpp"

Manager::Manager(std::string ID, std::string pin, Bank *bank) {

  this->ID = ID;
  this->pin = pin;
}

std::string Manager::getManagerID() { return this->ID; }
std::string Manager::getPin() { return this->pin; }

bool Manager::validatePin(std::string pin){

  return this->pin == pin;
}

void Manager::addAccount(BankAccount *account){

    this->accounts.push_back(account);
}

void Manager::addUser(User *user){

  this->users.push_back(user);
}
