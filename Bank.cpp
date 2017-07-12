#include "Bank.hpp"

Bank::Bank(std::string name) { this->name = name; }

std::string Bank::getNewUserID() {

  std::string ID;
  int len = 6;
  bool nonUnique;

  // contnue looping until we get a unique ID
  do {

    ID = "";
    for (int i = 0; i < len; i++) {
      ID += std::to_string(rand() % 11);
    }

    // check if unique
    nonUnique = false;
    for (User *u : this->users) {
      if (ID == u->getID()) {
        nonUnique = true;
        break;
      }
    }

  } while (nonUnique);

  return ID;
}
std::string Bank::getNewAccountID() {
  std::string ID;
  int len = 10;
  bool nonUnique;

  // contnue looping until we get a unique ID
  do {

    ID = "";
    for (int i = 0; i < len; i++) {
      ID += std::to_string(rand() % 11);
    }

    // check if unique
    nonUnique = false;
    for (BankAccount *a : this->accounts) {
      if (ID == a->getID()) {
        nonUnique = true;
        break;
      }
    }

  } while (nonUnique);

  return ID;
}
void Bank::addUser(User *user) { this->users.push_back(user); }
Manager *Bank::addManager(std::string ID, std::string pin) {

  Manager *m1 = new Manager(ID, pin, this);
  this->managers.push_back(m1);

  // write  the contents of the manager vector into a file so it remembers the
  // id  for next time
  std::ofstream file(ID);

  for (std::vector<Manager *>::iterator itr = this->managers.begin();
       itr != this->managers.end(); ++itr) {

    file << (*itr)->getManagerID() << '\n';
    file << (*itr)->getPin() << '\n';
  }
  return m1;
}

User *Bank::giveUser(std::string ID) {

  // for (std::vector<User *>::iterator itr = this->users.begin();
  //      itr != this->users.end(); ++itr) {
  //   if ((*itr)->getID()== ID)
  //     return *itr;
  // }
  // return nullptr;
  // std::string name;     // line 1 is the first name in a text file
  // std::string password; // password
  std::ifstream custFile;
  custFile.open(ID + ".txt");
  bool fileFound = true;

  // gets the 2nd line which is the password
  if (!custFile) {

    fileFound = false;
    return nullptr;
  } else {
    // file was found

    User *user = new User(ID);
    // user->setID(ID);
    // std::cout<<user->getID()<<std::endl;
    custFile.close();
    return user;
  }
}
User *Bank::addUser(std::string name, std::string pin) {
  // std::string account;
  // User *newUser;

  User *newUser = new User(name, pin, this);
  this->users.push_back(newUser);

  // creaete type of  account for the user and add to User and bank account
  // lists

  BankAccount *newAccount = new BankAccount("Saving", newUser, this);
  newUser->addAccount(newAccount);
  this->accounts.push_back(newAccount);

  return newUser;
}
Manager *Bank::managerLogin(std::string ID, std::string pin) {

  for (Manager *m : this->managers) {
    if (m->getManagerID() == ID && m->validatePin(pin)) {
      return m;
    }
  }
  return nullptr;
}
void Bank::addManager(Manager *manager) { this->managers.push_back(manager); }
User *Bank::userLogin(std::string ID, std::string pin) {

  // Search through list of users
  std::string name;     // line 1 is the first name in a text file
  std::string password; // password
  std::ifstream custFile;
  custFile.open(ID + ".txt");
  bool fileFound = true;

  // gets the 2nd line which is the password
  if (!custFile) {

    fileFound = false;
    return nullptr;
  } else if (fileFound) {
    // file was found (matching ID's), time to check if the pin is right also
    getline(custFile, name);
    getline(custFile, password);
    if (password == pin) {

      User *user = new User(name, pin, ID);
      // user->setID(ID);
      // std::cout<<user->getID()<<std::endl;
      custFile.close();
      return user;
    }
  }
  custFile.close();
  return nullptr;

  // for (User *u : this->users) {
  //
  //   // check user ID is correct
  //
  //   if (u->getID() == ID && u->validatePin(pin)) {
  //     return u;
  //   }
  // }
  // return nullptr;
}
void Bank::printCustomerAccountDetails() {

  for (std::vector<User *>::iterator itr = this->users.begin();
       itr != this->users.end(); ++itr) {

    (*itr)->printAllAcctsTransHistory();
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
std::string Bank::getName() { return this->name; }
void Bank::addAccount(BankAccount *anAcct) { this->accounts.push_back(anAcct); }
