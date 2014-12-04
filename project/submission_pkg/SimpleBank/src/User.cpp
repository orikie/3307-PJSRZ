//
//  User.cpp
// 
//  Created: 2014-09-16
//

#include "User.h"

/**
 * USER
 */
SB::User::User()
{
    init("3307");
    //this->utype = UserType::MGR;
    del_uid = -1;
    
}

SB::User::~User()
{
}


SB::User:: User(const string & id):utype(User::CLIENT)
{
    init(id);
}

string SB::User::usertypeToString(SB::User::UserType u)
{
    string utype = "";
    switch (u) {
        case Client::CLIENT:
            utype = "Client";
            break;
        case Client::MGR:
            utype = "Manager";
            break;
        case Client::MNT:
            utype = "Maintenance";
        default:
            break;
    }
    return utype;
}

void SB::User::init(const string &id)
{
    this->id_ = id;
    this->pass_ = DEFAULT_PASSWORD;
    
    this->privilege_ = User::PRI_1;
    this->newUser_ = true;

}

string SB::User::getID()
{
    return this->id_;
}

string SB::User::getPass()
{
    return this->pass_;
}

void SB::User::changePasswordHash(string h)
{
    this->pass_ = h;
}

void SB::User::changePassword(string s)
{
    this->pass_ = Utils::HashPassword(s);
    newUser_ = false;
}

/**
 * CLIENT
 */
SB::Client::Client()
{
    
}

SB::Client::Client(const string & id):User(id)
{
    this->utype = User::CLIENT;
    
}

SB::Client::Client(const string & uid,const string & passHash,const double& savings, const double& checking):User(uid)
{
    this->changePasswordHash(passHash);
    this->saving_ = SavingAccount(savings);
    this->checking_ = CheckingAccount(checking);
}

SB::Client::~Client()
{

}

bool SB::Client::isCloseable()
{
    return ((getCheckingBalance() <= 0) && (getSavingsBalance() <= 0));
}

void SB::Client::openSavings()
{
    this->saving_.activateAccount(true);
}

void SB::Client::openChecking()
{
    this->checking_.activateAccount(true);
}

void SB::Client::closeChecking()
{
    this->saving_.activateAccount(false);
}

void SB::Client::closeSavings()
{
    this->saving_.activateAccount(false);
}

bool SB::Client::withdrawSaving(double amt, double& rem)
{
    return this->saving_.withdraw(amt, rem);
}

bool SB::Client::withdrawChecking(double amt, double& rem)
{
    return this->checking_.withdraw(amt, rem);
}

bool SB::Client::depositChecking(double amt, double& rem)
{
    return this->checking_.deposit(amt, rem);
}

bool SB::Client::depositSaving(double amt, double& rem)
{
    return this->saving_.deposit(amt, rem);
}

/**
 * MAINTENANCE
 */
SB::Maintenance::Maintenance(const string &id) : User(id)
{
    this->privilege_ = User::PRI_2;
    this->utype = User::MNT;
}

SB::Maintenance::~Maintenance()
{

}

/**
 * MANAGER
 */
SB::Manager::Manager(const string &id):User(id)
{
    this->privilege_ = User::PRI_3;
    this->utype = User::MGR;
}

SB::Manager::~Manager()
{

}


