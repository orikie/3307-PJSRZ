//
//  User.h
//
//  Created: 2014-09-16
//

#ifndef __SimpleBank__User__
#define __SimpleBank__User__

#include <string>
#include "BankAccount.h"
#include "Utils.h"
#include "CONFIG.h"

using namespace std;

namespace SB {
    //BASE

    class User
    {
    public:
        
        enum UserType
        {
            CLIENT = 0, //CLIENT
            MGR,        //MANAGER
            MNT         //MAINTENANCE
        };
        
        //PRIVILEGE LEVEL: 1 = LOWEST
        enum PRI {
            PRI_1,
            PRI_2,
            PRI_3
        };
        
        
        User();
        ~User();
        User(const string &);
        
        void init(const string &);
        string getID();
        string getPass();
        void changePassword(string);
        void changePasswordHash(string);
        UserType getUserType() {return this->utype;}
        
        bool isNewUser() {return newUser_; }
        
        static string usertypeToString(UserType);
        
        int del_uid;
        
    protected:
        PRI privilege_;
        UserType utype;
        
    private:
        string id_;
        string pass_;
        
        bool newUser_;
        
    };
    
    //CLIENT
    class Client : public User
    {
    public:
        Client();
        Client(const string &);
        Client(const string &,const string &,const double&, const double&);
        ~Client();
        
        void openSavings();
        void openChecking();
        
        void closeSavings();
        void closeChecking();
        
        bool isSavingOpened()   { return this->saving_.isAccountActivated(); }
        bool isCheckingOpened() { return this->checking_.isAccountActivated(); }
        
        double getSavingsBalance() {return saving_.checkBalance();}
        double getCheckingBalance() {return checking_.checkBalance();}
        
        void activateSaving(){saving_.activateAccount(true);}
        void activateChecking(){checking_.activateAccount(true);}
        
        bool withdrawSaving(double, double&);
        bool withdrawChecking(double, double&);
        bool depositSaving(double, double&);
        bool depositChecking(double, double&);
        
        bool isCloseable();
        
        
        
    private:
        BankAccount saving_;
        BankAccount checking_;
        CreditAccount cc_;
    };
    
    //MANAGER
    class Manager : public User
    {
    public:
        Manager(const string &id);
        ~Manager();
    };
    
    //MAINTENANCE
    class Maintenance : public User
    {
    public:
        Maintenance(const string & id);
        ~Maintenance();
    };
}


#endif /* defined(__SimpleBank__User__) */
