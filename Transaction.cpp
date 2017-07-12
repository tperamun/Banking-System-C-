#include "Transaction.hpp"

Transaction::Transaction(std::string userID, std::string account,
                         std::string memo, double amount) {
  this->ID = userID;
  this->account = account;
  this->memo = memo;

  this->amount = amount;
  this->timeStamp = time(0); // get current time and date
  this->dt = ctime(&timeStamp);

  writeToFile(this->ID, this->account, this->dt, this->memo, this->amount);
  // std::ofstream temp("temp.txt")
}
void Transaction::writeToFile(std::string ID, std::string account, char *dt,
                              std::string memo, double amount) {
std::cout << "gangggg\n";
  std::string line;
  std::ifstream transFile;
  bool userFound = false;
  bool userAccountFound = false;
  std::string filename = "activity.txt";
  transFile.open(filename);
  std::ofstream temp("temp.txt");
  // std::cout << "im here " << std::endl;
  while (getline(transFile, line)) {
    temp << line << std::endl;
    // temp<< "im here also hereeee " << std::endl;
    unsigned delim1 = line.find(";");
    std::string userid = line.substr(0, delim1);

    if (userid == ID) {
      userFound = true;
      while (getline(transFile, line)) {
        temp << line << std::endl;
        unsigned delim2 = line.find(".");
        std::string accountName = line.substr(0, delim2);
        if (accountName == account) {
          userAccountFound = true;
          while (getline(transFile, line)) {
            temp << memo << "  $" << amount << " "<<dt << std::endl;
            temp << line << std::endl;
          }
        }
      }
    }
  }
  if (!userFound) {
    // this means its a new user
    bool contentExists = false;
    // std::cout << "gangggg\n";
    transFile.clear();
    transFile.seekg(0, std::ios::beg);
    while (getline(transFile, line)) {
      contentExists = true;
      temp << ID << ";" << std::endl;
      temp << account << "." << std::endl;
      temp << memo << "  $" << amount << " "<<dt;
      temp << "--" << std::endl;
    }

    if (!contentExists) {
      temp << ID << ";" << std::endl;
      temp << account << "." << std::endl;
      temp << memo << "  $" << amount <<" "<< dt << std::endl;
      temp << "--" << std::endl;
    }
  }

  if (userFound && !userAccountFound) {
    transFile.clear();
    transFile.seekg(0, std::ios::beg);

    while (getline(transFile, line)) {
      temp << line << std::endl;
      unsigned delim1 = line.find(";");
      std::string userid = line.substr(0, delim1);
      if (userid == ID) {

        while (getline(transFile, line)) {
          temp << account << "." << std::endl;
          temp << memo << " " << dt << amount << std::endl;
          temp << "--" << std::endl;
          temp << line << std::endl;
        }
      }
    }
  }

  temp.close();
  transFile.close();

  remove(filename.c_str());
  rename("temp.txt", filename.c_str());
}

Transaction::Transaction(double amount, BankAccount *inAccount) {

  this->amount = amount;
  this->inAccount = inAccount;
  this->memo = "";
  this->timeStamp = time(0); // get current time and date
  this->dt = ctime(&timeStamp);
}

Transaction::Transaction(double amount, std::string memo,
                         BankAccount *inAccount)
    : Transaction(amount, inAccount) {

  // call previous constructor

  this->memo = memo;
}

Transaction::Transaction(std::string amount, std::string memo,
                         BankAccount *inAccount) {
  this->amount1 = amount;
  this->inAccount = inAccount;
  this->memo = memo;
}

double Transaction::getAmount() { return this->amount; }

std::string Transaction::getSummaryLine() {

  if (this->amount >= 0) {
    std::stringstream ss;
    ss << this->dt << ":"
       << ":$" << std::setprecision(2) << this->amount << ": " << this->memo;
    return ss.str();
  } else {
    std::stringstream ss;
    ss << this->dt << ": "
       << ": $"
       << "(" << -this->amount << ")"
       << ": " << this->memo << ": ";
    return ss.str();
  }
}
