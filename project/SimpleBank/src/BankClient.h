//
//  BankClient.h
//  A console client that connects to the simple bank server
//  Both managers and clients can login
//  Menus differ depending on current user roles
//  Passes appropriate calls to SimpleBank instance
//
//  Created: 2014-09-19
//

#ifndef __SimpleBank__BankClient__
#define __SimpleBank__BankClient__

#include "SimpleBank.h"
#include "Utils.h"
#include "User.h"

using namespace std;
namespace SB{
    class BankClient
    {
    public:
        
        BankClient();
        ~BankClient();
        
        void start();
        
    private:
        //Main screen pre-login
        void welcomeScreen();
        
        //Client menu
        void clientMenu();
        void accessSavings();
        void accessChecking();
        void refreshCache();
        void refreshCache(int del_id_c);
        void accountAction(bool);
        void withdraw(bool);
        void deposit(bool);
        void transferFunds();
        void savingsToChecking();
        void checkingToSavings();
        void viewCreditPurchases();
        void printClientAccountBalances();
        void changeCreditOption();
        
        //Manager user menu
        void mgrMenu();
        void auditUser();
        bool auditUser(const string);
        void listAllInfo();
        void openNewUserAccount();
        void closeUserAccount();
        void bankStats();
        void triggerEndofMonth();
        void viewFailedCreditPayments();
        
        //MNT user menu
        void mntMenu();
        void setTraceSetting();
        //misc
        void clearScreen();
        bool login();
        void logout();
        void mainMenuInfo();
        
        void log(const string & s){logger_.logTrace(s);}
        void logError(const string & s) {logger_.logErr(s);}
        //util
        
        Client getcurrClient(){return bankServer_.getClient(userCache_.getID());}
        
        void changePassword();
        
        User userCache_;
        BankAccount activeAccountCache_;
        
        SimpleBank bankServer_;
        Logger logger_;

    };
}
#endif /* defined(__SimpleBank__BankClient__) */
