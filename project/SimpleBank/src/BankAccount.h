//
//  BankAccount.h
//
//  Created: 2014-09-16
//

#ifndef __SimpleBank__BankAccount__
#define __SimpleBank__BankAccount__

namespace SB
{
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
        
    private:
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

}


#endif /* defined(__SimpleBank__BankAccount__) */
