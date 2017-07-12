#include "User.hpp"
int numLinesInCustomerFile(std::string ID);
// #include "BankAccount.hpp"
User::User(std::string name, std::string pin, Bank *bank) {

  // this->firstName = firstName;
  // this->lastName = lastName;
  this->name = name;
  this->pin = pin;

  // get a new unique ID for the user
  this->ID = bank->getNewUserID();

  std::cout << "New User " << this->name << " with ID " << this->ID
            << " has been created" << std::endl;
}
User::User(std::string name, std::string pin, std::string ID) {
  std::string line;
  this->name = name;
  this->pin = pin;
  this->ID = ID;
  this->timeStamp = time(0);
  this->dt = ctime(&timeStamp);
  std::ifstream logFile;
  std::string filename = "log.txt";
  logFile.open(filename);
  std::ofstream temp("temp.txt");

  temp << this->ID << ":      " << this->dt << "     Customer signed on "
       << std::endl;
  while (getline(logFile, line)) {

    temp << line << std::endl;
  }

  temp.close();
  logFile.close();
  remove(filename.c_str());
  rename("temp.txt", filename.c_str());
  std::cout << "User " << this->name << " with ID " << this->ID << std::endl;
}
User::User(std::string ID) { this->ID = ID; }
// unused for now

void User::printAccountsSummary() {
  int i = 0;
  printf("\n\n%s's accounts summary\n", this->name.c_str());

  for (std::vector<BankAccount *>::iterator itr = this->accounts.begin();
       itr != this->accounts.end(); ++itr) {

    printf("  %d) %s\n", ++i, (*itr)->getSummaryLine().c_str());
  }

  std::cout << std::endl;
}

void User::printAccountsSummary(std::string ID) {
  int i = 0;
  std::string line; // line 1 is the first name in a text file
  // std::string password; // password
  std::ifstream custFile;
  custFile.open(ID + ".txt");

  printf("\n\n%s's accounts summary\n", this->name.c_str());

  for (int linenum = 0;
       getline(custFile, line) && linenum < numLinesInCustomerFile(ID);
       linenum++) {

    if (linenum == 0 || linenum == 1)
      continue;

    std::cout << ++i << ")" << line << std::endl;
  }

  std::cout << std::endl;
}

// void User::setID(std::string ID){
//   this->ID= ID;
// }
std::string User::getPin() { return this->pin; }
void User::addAccount(BankAccount *anAcct) { this->accounts.push_back(anAcct); }

std::string User::getID() { return this->ID; }

double User::getAcctBalance(int acctIdx) {
  return this->accounts.at(acctIdx)->getBalance();
}
bool User::validatePin(std::string pin) { return pin == this->pin; }
void User::removeAccount(std::string name) {
  bool isZero = false;
  for (std::vector<BankAccount *>::iterator itr = this->accounts.begin();
       itr != this->accounts.end(); ++itr) {
    if ((*itr)->getBankAccountName() == name) {

      if ((*itr)->getBalance() == 0) {
        isZero = true;
        this->accounts.erase(itr);
        std::cout << "Account successfully removed" << std::endl;
      }
    }
  }

  if (!isZero) {
    std::cout << "Account could not be removed. Balance must be $0 for the "
                 "account to be removed"
              << std::endl;
  }
}

bool User::accountExists(std::string name) {

  for (std::vector<BankAccount *>::iterator itr = this->accounts.begin();
       itr != this->accounts.end(); ++itr) {

    if ((*itr)->getBankAccountName() == name)
      return true;
  }
  return false;
}
int User::numAccounts() { return this->accounts.size(); }

std::string User::getName() { return this->name; }

void User::printAcctTransHistory(int acctIdx) {
  this->accounts.at(acctIdx)->printTransHistory();
}

void User::printAllAcctsTransHistory() {

  for (int i = 0; i < this->accounts.size(); i++) {

    this->accounts.at(i)->printTransHistory();
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

std::string User::getAcctID(int acctIdx) {
  return this->accounts.at(acctIdx)->getID();
}
void User::addAcctTransactions(std::string acctIdx, double amount,
                               std::string memo) {

  // this->accounts.at(acctIdx)->addTransaction(amount, memo);
}
// int numLinesInCustomerFile(std::string ID) {
//
//   int numLines = 0;
//   std::string line;
//   std::ifstream custFile;
//   custFile.get(ID+".txt");
//   while (std::getline(custFile, line))
//     ++numLines;
//
//   return numLines;
// }
