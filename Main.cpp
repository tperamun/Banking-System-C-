
#include "BankAccount.hpp"
#include "Manager.hpp"
#include "User.hpp"
#include <cmath>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <ncurses.h>
#include <string.h>
// Manager *managerMenuPrompt(Bank *bank, std::istream &stream);
// void mainMenuPrompt(Bank *bank, std::istream &stream);
void mainMenuPrompt(Bank *bank, User *theUser, Manager *theManager,
                    std::istream &stream);
void printUserMenu(User *theUser, std::istream &stream);
void printManagerMenu(Manager *theManager, std::istream &stream, Bank *bank);
void transferFunds(User *theUser, std::istream &stream);
void depositFunds(User *theUser, std::istream &stream);
void withDraw(User *theUser, std::istream &stream);
void showTransHistory(User *theUser, std::istream &stream);
User *createCustomer(Bank *bank, Manager *manager);
void openCustomerAccount(Bank *bank, Manager *manager);
void closeCustomerAccount(Bank *bank, Manager *manager);
void viewCustomerAccountDetails(Bank *bank);
void viewAllCustomerDetails(Bank *bank);
int numLinesInCustomerFile(std::string ID);
void writeToLogFile(User *theUser, std::string action, std::string toOrFrom,
                    double amount);
void newCustomerFile(BankAccount *account);

int main(int argc, char const *argv[]) {

  std::srand(time(NULL)); // generate a new random ID this way
  Bank *bank = new Bank("Bank of Timmy");
  User *curUser;
  Manager *curManager;
  bank->addManager("manager1", "1234"); // add manager with w.e id/pin

  mainMenuPrompt(bank, curUser, curManager, std::cin);
  //
  // if (curUser != nullptr)
  //   printUserMenu(curUser, std::cin);
  // if (curManager != nullptr) {
  //   bank->addManager(curManager);
  //   printManagerMenu(curManager, std::cin, bank);
  // }
  // else if(curManager == nullptr)
  //   std::cout<<"hiiii"<<std::endl;
}

void mainMenuPrompt(Bank *bank, User *theUser, Manager *theManager,
                    std::istream &stream) {

  std::string ID;
  std::string pin;
  // bool userFound = false;
  // bool managerFound = false;
  // prompt the user for the user ID/Pin combo until the correct one has been
  // entered
  do {

    std::cout << "\n\nWelcome to " << bank->getName() << std::endl;
    std::cout << "Enter  ID: " << std::endl;
    stream >> ID;
    std::cout << "Enter pin: ";
    stream >> pin;

    // try to get the user object corresoponding to the ID and Pin combo
    theUser = bank->userLogin(ID, pin);
    theManager = bank->managerLogin(ID, pin);

    if (theUser == nullptr && theManager == nullptr) {
      std::cout << "Incorrect user ID/Pin combination, Please try again. "
                << std::endl;
    }

  } while (theUser == nullptr &&
           theManager == nullptr); // keep going until valid login

  if (theUser != nullptr)
    printUserMenu(theUser, std::cin);
  if (theManager != nullptr) {
    bank->addManager(theManager);
    std::string line;
    time_t timeStamp = time(0);
    char *dt = ctime(&timeStamp);
    std::ifstream logFile;
    std::string filename = "log.txt";
    logFile.open(filename);
    std::ofstream temp2("temp.txt");

    temp2 << theManager->getManagerID() << ":      " << dt
          << "     Manager1 signed on" << std::endl;

    while (getline(logFile, line)) {

      temp2 << line << std::endl;
    }

    temp2.close();
    logFile.close();
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
    printManagerMenu(theManager, std::cin, bank);
  }
}

void printManagerMenu(Manager *theManager, std::istream &stream, Bank *bank) {
  int choice;

  do {
    std::cout << "Welcome " << theManager->getManagerID()
              << " what would you like to do?" << std::endl;
    std::cout << "\n1. Create customer\n2. Open Account for Customer\n3. Close "
                 "Account for Customer\n4. See account details of a specific "
                 "Customer\n"
                 "5. See account details of all customers\n6. Log off "
              << std::endl;

    std::cout << "\nEnter Choice: ";
    stream >> choice;
    if (choice < 1 || choice > 6) {
      std::cout << "Invalid choice. Please choose 1-5" << std::endl;
    }

  } while (choice < 1 || choice > 6);

  User *newUser;
  switch (choice) {
  case 1:
    newUser = createCustomer(bank, theManager);
    break;
  case 2:
    openCustomerAccount(bank, theManager);
    break;
  case 3:
    closeCustomerAccount(bank, theManager);
    break;
  case 4:
    viewCustomerAccountDetails(bank);
    break;
  case 5:
    viewAllCustomerDetails(bank);
    break;
  case 6:
    std::string line;
    time_t timeStamp = time(0);
    char *dt = ctime(&timeStamp);
    std::ifstream logFile;
    std::string filename = "log.txt";
    logFile.open(filename);
    std::ofstream temp2("temp.txt");

    temp2 << theManager->getManagerID() << ":      " << dt
          << "     Manager1 logged off" << std::endl;

    while (getline(logFile, line)) {

      temp2 << line << std::endl;
    }

    temp2.close();
    logFile.close();
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
    break;
  }
  if (choice != 6)
    printManagerMenu(theManager, std::cin, bank);
}
void viewAllCustomerDetails(Bank *bank) { bank->printCustomerAccountDetails(); }
void viewCustomerAccountDetails(Bank *bank) {

  User *customer;
  std::string name;
  do {
    std::cout << "Enter customer's first name to view their account details: ";
    std::cin >> name;
    customer = bank->giveUser(name);
    if (customer == nullptr)
      std::cout << "Customer does not exist!" << std::endl;

  } while (customer == nullptr);

  customer->printAllAcctsTransHistory();
}

void closeCustomerAccount(Bank *bank, Manager *manager) {

  User *existingUser;
  bool accountIsZero = false;
  bool accountFound = false;
  std::string ID;
  do {

    std::cout << "Enter ID of customer :";
    std::cin >> ID;

    existingUser = bank->giveUser(ID);

    if (existingUser == nullptr)
      std::cout << "Customer does not exist!" << std::endl;
  } while (existingUser == nullptr);

  std::string accountType;
  ID = ID + ".txt";
  std::string line;

  std::ifstream custFile;
  custFile.open(ID);
  std::ofstream temp("temp.txt");
  std::string lineToDelete;
  do {

    std::cout << "Enter type of account to close: ";
    std::cin >> accountType;

    while (getline(custFile, line)) {
      // std::cout<<line<<std::endl;
      unsigned delim = line.find(":");
      std::string acct = line.substr(0, delim);

      std::string strBal = line.substr(delim + 1, line.length());

      if (accountType == acct) {
        double balance = stod(strBal);
        accountFound = true;

        if (balance == 0) {
          accountIsZero = true;
          lineToDelete = accountType + ":" + strBal;
        }
      }
      // temp<<line<<std::endl;
    }

    // std::cout<<accountIsZero<<std::endl;
    // while(getline(custFile,line)){
    //   std::cout<<line<<std::endl;}
    if (!accountIsZero) {
      custFile.clear();
      custFile.seekg(0, std::ios::beg);
      std::cout << "Account does not Exist or the accounts balance is greater "
                   "than zero! Please Try again"
                << std::endl;
    }

  } while (!accountIsZero);
  custFile.clear();
  custFile.seekg(0, std::ios::beg);

  while (getline(custFile, line)) {
    if (line == lineToDelete)
      continue;
    temp << line << std::endl;
  }

  temp.close();
  custFile.close();
  remove(ID.c_str());
  rename("temp.txt", ID.c_str());

  time_t timeStamp = time(0);
  char *dt = ctime(&timeStamp);
  std::ifstream logFile;
  std::string filename = "log.txt";
  logFile.open(filename);
  std::ofstream temp2("temp.txt");

  temp2 << manager->getManagerID() << ":      " << dt << "     Closed "
        << accountType << " account "
        << " for user with ID " << existingUser->getID() << std::endl;

  while (getline(logFile, line)) {

    temp2 << line << std::endl;
  }

  temp2.close();
  logFile.close();
  remove(filename.c_str());
  rename("temp.txt", filename.c_str());
}
void openCustomerAccount(Bank *bank, Manager *manager) {
  User *existingUser;

  std::string ID;
  do {

    std::cout << "Enter customer ID :";
    std::cin >> ID;

    existingUser = bank->giveUser(ID);

    if (existingUser == nullptr)
      std::cout << "Customer does not exist!" << std::endl;
  } while (existingUser == nullptr);

  std::string accountType;
  std::string line;
  std::ifstream custFile;
  ID = ID + ".txt";

  do {

    std::cout << "Enter type of account to open: ";
    std::cin >> accountType;

    // std::ofstream temp("temp.txt");

    custFile.open(ID);

    while (getline(custFile, line)) {
      if (line.find(accountType) != std::string::npos) {
        std::cout << "Account already exists!" << std::endl;
      }
    }

  } while (line.find(accountType) != std::string::npos);
  custFile.close();

  std::ofstream ofs;
  ofs.open(ID, std::ofstream::out | std::ofstream::app);
  ofs << accountType << ":0" << std::endl;

  ofs.close();
  BankAccount *newAccount = new BankAccount(accountType, existingUser, bank);
  existingUser->addAccount(newAccount);
  bank->addAccount(newAccount);
  manager->addAccount(newAccount);
  /////////////////////////
  // std::string line;
  time_t timeStamp = time(0);
  char *dt = ctime(&timeStamp);
  std::ifstream logFile;
  std::string filename = "log.txt";
  logFile.open(filename);
  std::ofstream temp("temp.txt");

  temp << manager->getManagerID() << ":      " << dt << "     Opened "
       << accountType << " account "
       << " for user with ID " << existingUser->getID() << std::endl;

  while (getline(logFile, line)) {

    temp << line << std::endl;
  }

  temp.close();
  logFile.close();
  remove(filename.c_str());
  rename("temp.txt", filename.c_str());
}

//

void newCustomerFile(BankAccount *account) {
  std::string line;
  std::ifstream custFile;
  std::ofstream temp("temp.txt");
  std::string ID = account->getHolder()->getID() + ".txt";

  custFile.open(ID);

  temp << account->getAccountHolderName() << '\n'
       << account->getAccountHolderPin() << '\n'
       << account->getBankAccountName() << ":" << account->getBalance()
       << std::endl;

  temp.close();
  custFile.close();
  remove(ID.c_str());
  rename("temp.txt", ID.c_str());
}
void printUserMenu(User *theUser, std::istream &stream) {
  // print summary of the user's accounts

  theUser->printAccountsSummary(theUser->getID());

  int choice;
  // user menu

  do {

    std::cout << "Welcome " << theUser->getName()
              << "  what would you like to do?\n"
              << std::endl;
    std::cout << "1. Show account transaction history\n2. Withdraw\n3. "
                 "Deposit\n4. Transfer\n5. Log out"
              << std::endl;
    std::cout << "Enter choice: ";
    stream >> choice;

    if (choice < 1 || choice > 5) {
      std::cout << "Invalid choice. Please choose 1-5" << std::endl;
    }

  } while (choice < 1 || choice > 5);

  switch (choice) {
  case 1:
    showTransHistory(theUser, std::cin);
    break;
  case 2:
    withDraw(theUser, std::cin);
    break;
  case 3:
    depositFunds(theUser, std::cin);
    break;
  case 4:
    transferFunds(theUser, std::cin);
    break;
  case 5:
    std::string line;
    time_t timeStamp = time(0);
    char *dt = ctime(&timeStamp);
    std::ifstream logFile;
    std::string filename = "log.txt";
    logFile.open(filename);
    std::ofstream temp("temp.txt");

    temp << theUser->getID() << ":      " << dt << "     Customer logged off "
         << std::endl;
    while (getline(logFile, line)) {

      temp << line << std::endl;
    }

    temp.close();
    logFile.close();
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());

    break;
  }
  // redesplay menu
  if (choice != 5)
    printUserMenu(theUser, std::cin);
}

void showTransHistory(User *theUser, std::istream &stream) {

  std::string line;
  bool accountFound = false;
  std::ifstream activityFile;
  std::string ID = theUser->getID();
  activityFile.open("activity.txt");
  std::string accountName;
  std::cout << "Enter the name of the account you wish to see the transaction "
               "history of: ";
  stream >> accountName;
  std::cout << std::endl;
  while (getline(activityFile, line)) {

    unsigned delim1 = line.find(";");
    std::string userid = line.substr(0, delim1);

    if (ID == userid) {

      do {

        while (getline(activityFile, line)) {

          unsigned delim2 = line.find(".");
          std::string account = line.substr(0, delim2);

          if (account == accountName) {
            accountFound = true;
            std::cout << "Transaction History in your " << accountName
                      << " account\n\n";
            while (getline(activityFile, line) && (line != "--")) {
              std::cout << " " << line << std::endl;
            }
          }
        }
        if (!accountFound) {
          std::cout << "Invalid Account. Please try again.\n\nEnter the name "
                       "of the account you wish to see the transaction history "
                       "of: "
                    << std::endl;
          activityFile.clear();
          activityFile.seekg(0, std::ios::beg);
        }
      } while (!accountFound);
    }
  }
}

void transferFunds(User *theUser, std::istream &stream) {
  std::string line;
  std::string fromAcct;
  std::string toAcct;
  double amount;
  double acctBal;
  std::ifstream custFile;

  std::string ID = theUser->getID() + ".txt";
  custFile.open(ID);
  bool fromAcctFound = false;
  bool toAcctFound = false;
  std::string memo;
  int choice;
  double fromAcctBalance;
  double toAcctBalance;

  // get the accounts to transfer from

  do {
    std::cout << "Enter the account name you wish to transfer From: ";
    stream >> fromAcct;
    while (getline(custFile, line)) {

      unsigned first = line.find(":");
      std::string n = line.substr(0, first);
      if (n == fromAcct) {
        fromAcctFound = true;
        unsigned strnum = line.find(":");
        std::string strCurBalance = line.substr(strnum + 1, line.length());
        fromAcctBalance = stod(strCurBalance);
      }
    }
    if (!fromAcctFound) {
      std::cout << "The account you're trying to transfer from does not "
                   "exist. Please Try again!\n";
      custFile.clear();
      custFile.seekg(0, std::ios::beg);
    }
  } while (!fromAcctFound);

  custFile.clear();
  custFile.seekg(0, std::ios::beg);
  do {
    std::cout << "Enter the account name you wish to transfer To: ";
    stream >> toAcct;

    while (getline(custFile, line)) {

      unsigned first = line.find(":");
      std::string n = line.substr(0, first);
      if (n == toAcct) {
        toAcctFound = true;
        unsigned strnum = line.find(":");
        std::string strCurBalance = line.substr(strnum + 1, line.length());
        toAcctBalance = stod(strCurBalance);
      }
    }

    if (!toAcctFound) {
      std::cout << "The account you're trying to transfer to does not exist. "
                   "Please Try again!\n";
      custFile.clear();
      custFile.seekg(0, std::ios::beg);
    }
  } while (!toAcctFound);

  if (fromAcct == toAcct) {
    std::cout << "Woops! The two accounts provided were the same. Please try "
                 "again!\n\n";
    transferFunds(theUser, std::cin);
  }

  else {
    do {
      std::cout << "Enter the amount you would like to transfer from "
                << fromAcct << " to " << toAcct << ": ";
      stream >> amount;
      if (fromAcct == "Checking" && (fromAcctBalance - amount) < 1000.00) {
        do {
          // std::cout<<"hello"<<std::endl;

          std::cout << "Your balance on your Checking account will drop below "
                       "$1000.00. If you wish to continue, you will be "
                       "charged $2.00\n\n"
                       "Do you still wish to continue?\n\n1. Yes 2. No\n";
          stream >> choice;
          if (choice == 2)
            break;
          if (choice < 1 || choice > 2) {
            std::cout << "Select 1 if you wish to continue, select 2 if you "
                         "don't wish to continue. Try again: ";
          }
        } while (choice < 1 || choice > 2);
      }
      // std::cout<<toAcctBalance<<std::endl<< fromAcctBalance<<std::endl;
      toAcctBalance += amount;
      fromAcctBalance -= amount;

      std::string newFromAcctAmount =
          fromAcct + ":" + std::to_string(fromAcctBalance);
      std::string newToAcctAmount =
          toAcct + ":" + std::to_string(toAcctBalance);
      custFile.clear();
      custFile.seekg(0, std::ios::beg);
      std::ofstream temp("temp.txt");
      while (getline(custFile, line)) {
        if (line.find(fromAcct) != std::string::npos ||
            line.find(toAcct) != std::string::npos) {
          continue;
        } else {
          temp << line << std::endl;
        }
      }
      temp << newToAcctAmount << std::endl;
      temp << newFromAcctAmount << std::endl;

      temp.close();
      custFile.close();
      remove(ID.c_str());
      rename("temp.txt", ID.c_str());

      if (amount <= 0) {
        std::cout << "Amount to transfer must be greater than 0\nPlease Try "
                     "Again\n\n";
      }
    } while (amount <= 0);
  }

  std::cout << "Enter memo: ";
  stream >> memo;

  writeToLogFile(theUser, "Transfered from ",
                 fromAcct + "'s account to " + toAcct, amount);
}

void withDraw(User *theUser, std::istream &stream) {

  std::string line;
  std::string fromAcct;
  double amount;
  std::ifstream custFile;
  std::string ID = theUser->getID() + ".txt";
  custFile.open(ID);
  bool found = false;
  std::string memo;
  int choice;
  // get the accounts to transfer from
  double newBal;
  std::cout << "Enter account name to withdraw from: ";
  stream >> fromAcct;

  while (getline(custFile, line)) {

    unsigned first = line.find(":");
    std::string n = line.substr(0, first);

    if (n == fromAcct) {
      found = true;
      unsigned strnum = line.find(":");
      std::string strCurBalance = line.substr(strnum + 1, line.length());
      double curBal = stod(strCurBalance);

      do {
        std::cout << "Enter amount to withdraw: ";
        stream >> amount;

        newBal = curBal - amount;

        std::string newAmount = fromAcct + ":" + std::to_string(newBal);
        custFile.clear();
        custFile.seekg(0, std::ios::beg);
        std::ofstream temp("temp.txt");
        while (getline(custFile, line)) {
          if (line.find(fromAcct) != std::string::npos) {
            continue;
          } else {
            temp << line << std::endl;
          }
        }
        temp << newAmount << std::endl;

        temp.close();
        custFile.close();
        remove(ID.c_str());
        rename("temp.txt", ID.c_str());
        if (fromAcct == "Checking" && (curBal - amount) < 1000.00) {

          do {

            std::cout << "Your balance on your account will drop below "
                         "$1000.00. If you wish to continue, you will be "
                         "charged $2.00\n\n"
                         "Do you still wish to continue?\n\n1. Yes 2. No\n";
            stream >> choice;
            if (choice == 2)
              break;
            if (choice < 1 || choice > 2) {
              std::cout << "Select 1 if you wish to continue, select 2 if you "
                           "don't wish to continue. Try again: ";
            }
          } while (choice < 1 || choice > 2);
        }

        if (amount <= 0) {
          std::cout << "Amount must be greater than zero." << std::endl;
        }

      } while (amount <= 0);
    }
  }
  if (!found) {
    std::cout << "Invalid account. Please try again." << std::endl;

    withDraw(theUser, std::cin);
  }
  // std::cout << "Enter a memo: ";
  // stream >> memo;

  // new Transaction(theUser->getID(), fromAcct, memo, amount);
  writeToLogFile(theUser, "Withdrew from", fromAcct, amount);
}
void writeToLogFile(User *theUser, std::string action, std::string toOrFrom,
                    double amount) {
  time_t timeStamp = time(0);
  char *dt = ctime(&timeStamp);
  std::string line;
  std::ifstream logFile;
  std::string filename = "log.txt";
  logFile.open(filename);
  std::ofstream temp("temp.txt");

  temp << theUser->getID() << ":      " << dt << "     " << action << " "
       << toOrFrom << "'s account in the amount of $" << amount << std::endl;

  while (getline(logFile, line)) {

    temp << line << std::endl;
  }

  temp.close();
  logFile.close();
  remove(filename.c_str());
  rename("temp.txt", filename.c_str());
}
void depositFunds(User *theUser, std::istream &stream) {
  std::string line;
  std::string toAcct;
  double amount;
  std::ifstream custFile;
  std::string ID = theUser->getID() + ".txt";
  custFile.open(ID);
  bool found = false;
  std::string memo;
  double newBal;

  std::cout << "Enter account name to deposit to: ";
  stream >> toAcct;

  while (getline(custFile, line)) {

    unsigned first = line.find(":");
    std::string n = line.substr(0, first);

    if (n == toAcct) {
      found = true;
      unsigned strnum = line.find(":");
      std::string strCurBalance = line.substr(strnum + 1, line.length());
      double curBal = stod(strCurBalance);

      do {
        std::cout << "Enter amount to deposit: ";
        stream >> amount;

        newBal = curBal + amount;
        std::string newAmount = toAcct + ":" + std::to_string(newBal);
        custFile.clear();
        custFile.seekg(0, std::ios::beg);
        std::ofstream temp("temp.txt");
        while (getline(custFile, line)) {
          if (line.find(toAcct) != std::string::npos) {
            continue;
          } else {
            temp << line << std::endl;
          }
        }
        temp << newAmount << std::endl;

        temp.close();
        custFile.close();
        remove(ID.c_str());
        rename("temp.txt", ID.c_str());
        if (amount <= 0) {
          std::cout << "Amount must be greater than zero." << std::endl;
        }

      } while (amount <= 0);
    }
  }
  if (!found) {
    std::cout << "Invalid account. Please try again." << std::endl;

    depositFunds(theUser, std::cin);
  }
  std::cout << "Enter a memo: ";
  stream >> memo;

  new Transaction(theUser->getID(), toAcct, memo, amount);
  writeToLogFile(theUser, "Deposited to", toAcct, amount);
}

User *createCustomer(Bank *bank, Manager *manager) {

  std::string name;
  // std::string lastName;
  std::string pin;
  std::string accountType;

  std::cout << "Enter customers name: ";
  std::cin >> name;

  std::cout << "Enter a pin for this customer: ";
  std::cin >> pin;
  User *newUser = new User(name, pin, bank);

  std::cout << "Enter type of account to open: ";
  std::cin >> accountType;
  std::cout << std::endl;
  BankAccount *account = new BankAccount(accountType, newUser, bank);
  newUser->addAccount(account);
  bank->addAccount(account);
  bank->addUser(newUser);
  manager->addAccount(account);
  manager->addUser(newUser);

  newCustomerFile(account); // only ever runs in this method lol

  return newUser;
}

int numLinesInCustomerFile(std::string ID) {

  int numLines = 0;
  std::string line;
  std::ifstream custFile;
  custFile.open(ID + ".txt");
  while (std::getline(custFile, line))
    ++numLines;

  return numLines;
}
