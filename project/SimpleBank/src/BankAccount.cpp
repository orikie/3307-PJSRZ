//
//  BankAccount.cpp
//
//  Created: 2014-09-16
//


#include "BankAccount.h"

SB::BankAccount::BankAccount()
{
    balance_ = 0;
    activated_ = false;
}

SB::BankAccount::BankAccount(double a)
{
    balance_ = a;
    activated_ = a >= 0 ? true: false;
}

SB::BankAccount::~BankAccount()
{

}

bool SB::BankAccount::withdraw(double amount, double & rem)
{
    if(this->balance_ >= amount)
    {
        this->balance_ -= amount;
        rem = this->balance_;
        return true;
    }
    else
    {
        return false;
    }
}

bool SB::BankAccount::deposit(double amount, double & rem)
{
    if(amount >= 0)
    {
        this->balance_ += amount;
        rem = this->balance_;
        return true;
    }
    else
    {
        return false;
    }
    
}

double SB::BankAccount::checkBalance()
{
    return this->balance_;
}

void SB::BankAccount::activateAccount(const bool & open)
{
    this->activated_ = open;
    balance_ = 0;
}

SB::SavingAccount::SavingAccount(double a): BankAccount(a){}
SB::SavingAccount::~SavingAccount(){}

SB::CheckingAccount::CheckingAccount(double a): BankAccount(a){}
SB::CheckingAccount::~CheckingAccount(){}

SB::CreditAccount::CreditAccount(){
    balance_ = 0;
    activated_ = true;
}
SB::CreditAccount::CreditAccount(double a): BankAccount(a){}
SB::CreditAccount::~CreditAccount(){};