#include "BankAccount.hpp"

BankAccount::BankAccount(std::string name, User *holder, Bank *bank) {

  this->name = name;
  this->accountHolder = holder;

  this->ID = bank->getNewAccountID();

  // add this account to the holder and the bank lists
}
std::string BankAccount::getAccountHolderName(){

  return this->accountHolder->getName();
}
// User *BankAccount::accountHolder(){
//
// }
User *BankAccount::getHolder(){
  return this->accountHolder;
}
std::string BankAccount::getAccountHolderPin(){
  return this->accountHolder->getPin();
}
std::string BankAccount::getID() { return this->ID; }
std::string BankAccount::getBankAccountName(){
  return this->name;
}
std::string BankAccount::getSummaryLine() {
  // get the account's balance

  double balance = this->getBalance();

  std::stringstream ss;
  if (balance >= 0) {

    ss << this->ID << ": $" << balance << ": " << this->name << std::endl;
    return ss.str();
  } else {
    ss << this->ID << ": $" << balance << ": " << this->name << std::endl;
    return ss.str();
  }
}
void BankAccount::printTransHistory() {
  printf("\nTransaction history for account %s ", this->ID.c_str());

  for (int t = this->transactions.size() - 1; t >= 0; t--) {

    std::cout << this->transactions.at(t)->getSummaryLine();
  }

  std::cout << std::endl;
}

void BankAccount::addTransaction(double amount, std::string memo) {

  Transaction *newTrans = new Transaction(amount, memo, this);
  this->transactions.push_back(newTrans);
}
double BankAccount::getBalance() {

  double balance = 0;

  for (Transaction *t : this->transactions) {

    balance += t->getAmount();
  }
  return balance;
}
