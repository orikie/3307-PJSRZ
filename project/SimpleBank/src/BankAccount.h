//
//  BankAccount.h
//
//  Created: 2014-09-16
//

#ifndef __SimpleBank__BankAccount__
#define __SimpleBank__BankAccount__

namespace SB
{
    enum AccountType
    {
        Savings = 0,
        Checking,
        Credit
    };
    
    class BankAccount
    {
    public:
        BankAccount();
        BankAccount(double);
        virtual ~BankAccount();
        
        
        virtual bool deposit(double, double &);
        
        virtual bool withdraw(double, double &);
        double checkBalance();
        
        void activateAccount(const bool &);
        bool isAccountActivated() {return activated_;}
        
    protected:
        double balance_;
        bool activated_;
    };
    
    class SavingAccount: public BankAccount
    {
    public:
        SavingAccount();
        SavingAccount(double);
        ~SavingAccount();
        
    };
    
    class CheckingAccount: public BankAccount
    {
    public:
        CheckingAccount();
        CheckingAccount(double);
        ~CheckingAccount();
        
    };
    
    class CreditAccount: public BankAccount
    {
    public:
        CreditAccount();
        CreditAccount(double);
        ~CreditAccount();
        
        bool isFrozen(){return activated_;}
        void setFrozen(bool b){activated_ = b;}
        
        void setLimit(double lim){creditLimit_ = lim;}
        double getLimit(){return creditLimit_;}
    private:
        double creditLimit_;
    };
}


#endif /* defined(__SimpleBank__BankAccount__) */
