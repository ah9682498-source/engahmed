#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
/*#include <thread>
#include <chrono>
#include <windows.h>*/

using namespace std;

// ==========================================
// 1. (Encapsulation & Abstraction)
// ==========================================
class Account {
private:
    string name;
    string accountNumber;
    string password;

protected:
    double balance;

public:
    Account(string name, string accountNumber, string password, double initialBalance) {
        this->name = name;
        this->accountNumber = accountNumber;
        this->password = password;
        balance = initialBalance;
    }

    virtual ~Account() {}

    string getAccountNumber() const { return accountNumber; }

    bool authenticate(string pass) const {
        return this->password == pass;
    }

    // دالة تغيير كلمة السر
    bool changePassword(string oldPass, string newPass) {
        if (this->password == oldPass) {
            this->password = newPass;
            cout << "\n Password changed successfully!\n";
            return true;
        } else {
            cout << "\n Error: Incorrect old password! Password was not changed.\n";
            return false;
        }
    }

    double getBalance() const { return balance; }

    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "\n Error: Invalid amount. Deposit must be greater than 0.\n";
            return false;
        }
        if (amount < 50) {
            cout << "\n Error: Minimum deposit amount is 50.\n";
            return false;
        }

        balance += amount;
        cout << "\n Deposit successful. Current balance: " << balance << "\n";
        return true;
    }

    virtual bool withdraw(double amount) = 0;

    virtual void displayInfo() const {
        cout << "\033[1;37m";
        cout << "\n=== Account Info ===" << endl;
        cout << "\033[1;31m"; // وراثه للون
        cout << "Name: " << name << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: " << balance << endl;
    }
};

// ==========================================
// 2. (Inheritance & Polymorphism)
// ==========================================

class SavingsAccount final : public Account {
public:
    SavingsAccount(string name, string accNum, string pass, double balance)
        : Account(name, accNum, pass, balance) {}

    bool withdraw(double amount) override final {
        if (amount <= 0) {
            cout << "\n Invalid amount.\n";
            return false;
        }
        if (balance - amount < 500) {
            cout << "\n Transaction Rejected: Savings account balance cannot fall below 500.\n";
            return false;
        }
        balance -= amount;
        cout << "\n Withdrawal successful. Remaining balance: " << balance << "\n";
        return true;
    }

    void displayInfo() const override final {
        Account::displayInfo();
        cout << "Account Type: Savings\n";
    }
};

class CheckingAccount final : public Account {
public:
    CheckingAccount(string name, string accNum, string pass, double balance)
        : Account(name, accNum, pass, balance) {}

    bool withdraw(double amount) override final {
        if (amount <= 0) {
            cout << "\n Invalid amount.\n";
            return false;
        }
        if (balance - amount < 0) {
            cout << "\n Transaction Rejected: Insufficient balance.\n";
            return false;
        }
        balance -= amount;
        cout << "\n Withdrawal successful. Remaining balance: " << balance << "\n";
        return true;
    }

    void displayInfo() const override final {
        Account::displayInfo();
        cout << "Account Type: Checking\n";
    }
};

// ==========================================
//  (ATM Management System)
// ==========================================
class ATMSystem final {
private:
    vector<Account*> database;

public:
    ~ATMSystem() {
        for (Account* acc : database) {
            delete acc;
        }
    }

    void createAccount() {
        string name, accNum, pass;
        double balance;
        int type;
        cout << "\033[1;37m";
        cout << "=== Create New Account ===\n";
        cout << "\033[1;36m";
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Account Number: "; cin >> accNum;
        cout << "Enter Password: "; cin >> pass;
        cout << "Enter Opening Balance: "; cin >> balance;
        cout << "Select Account Type (1: Savings, 2: Checking): "; cin >> type;

        if (type == 1) {
            database.push_back(new SavingsAccount(name, accNum, pass, balance));
            cout << "\n Savings Account created successfully!\n";
        } else if (type == 2) {
            database.push_back(new CheckingAccount(name, accNum, pass, balance));
            cout << "\n Checking Account created successfully!\n";
        } else {
            cout << "\n Error: Invalid Account Type Selected. Account creation failed!\n";
        }
    }

    Account* login() {
        string accNum, pass;
        cout << "\033[1;37m";
        cout << "\n=== Login ===\n";
        cout << "\033[1;36m";
        cout << "Account Number: "; cin >> accNum;
        cout << "Password: "; cin >> pass;

        for (Account* acc : database) {
            if (acc->getAccountNumber() == accNum && acc->authenticate(pass)) {
                return acc;
            }
        }
        cout << "\n Invalid Account Number or Password!\n";
        return nullptr;
    }

    // ترجع true لو المستخدم طلب العودة للقائمة الرئيسية (Logout)
    // ترجع false لو تم تغيير الباسورد وبنحتاج نطلب Login تاني مباشرة
    bool showUserMenu(Account* currentUser) {
        int choice = 0;
        while (true) {
            cout << "\033[1;37m";
            cout << "\n==== User Menu ====\n";
            cout << "\033[1;36m";
            cout << "1- Check Balance\n";
            cout << "2- Deposit\n";
            cout << "3- Withdraw\n";
            cout << "4- Display Info\n";
            cout << "5- Change Password\n";
            cout << "6- Logout\n";
            cout << "Select Choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "\033[1;32m";
                    cout << "\nCurrent Balance: " << currentUser->getBalance() << "\n";
                    cout << "\033[1;36m";
                    break;
                case 2: {
                    double amount;
                    cout << "Enter deposit amount: "; cin >> amount;
                    cout << "\033[1;33m";
                    currentUser->deposit(amount);
                    cout << "\033[1;36m";
                    break;
                }
                case 3: {
                    double amount;
                    cout << "Enter withdrawal amount: "; cin >> amount;
                    cout << "\033[1;33m";
                    currentUser->withdraw(amount);
                    cout << "\033[1;36m";
                    break;
                }
                case 4:
                    cout << "\033[1;32m";
                    currentUser->displayInfo();
                    cout << "\033[1;36m";
                    break;
                case 5: {
                    string oldPass, newPass;
                    cout << "\033[1;34m";
                    cout << "Enter Old Password: "; cin >> oldPass;
                    cout << "Enter New Password: "; cin >> newPass;
                    cout << "\033[1;36m";
                    if (currentUser->changePassword(oldPass, newPass)) {
                        return false; // نرجع false عشان نوديه لصفحة الـ Login مباشرة
                    }
                    break;
                }
                case 6:
                    cout << "\nLogged out successfully!\n";
                    return true; // نرجع true عشان يرجع للقائمة الرئيسية ATM SYSTEM
                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }
        }
    }
};


// ==========================================
// (Main Function)
// ==========================================
int main() {
    ATMSystem atm;
    int option;

    while (true) {
        system("color 0B");
        cout << "\033[1;37m";
        cout << "\n=== ATM SYSTEM ===\n";
        cout << "\033[1;36m";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. Exit System\n";
        cout << "Select Option: ";
        cin >> option;

        if (option == 1) {
            atm.createAccount();
        } else if (option == 2) {
            while (true) { // حلقة تكرار لضمان تحويل المستخدم لشاشة الـ Login تلقائياً بعد تغيير كلمة السر
                Account* user = atm.login();
                if (user != nullptr) {
                    bool returnToMainMenu = atm.showUserMenu(user);
                    if (returnToMainMenu) {
                        break; // يرجع للقائمة الرئيسية فقط لو اختار Logout
                    }
                    // لو رجعت false (يعني غير الباسورد) اللوب هيلف ويفتح صفحة الـ Login تاني فوراً
                } else {
                    break; // لو الدخول فشل يرجع للـ Main Menu
                }
            }
        } else if (option == 3) {
            cout << "\nExiting System. Have a nice day!\n";
            break;
        }
    }

    return 0;
}
