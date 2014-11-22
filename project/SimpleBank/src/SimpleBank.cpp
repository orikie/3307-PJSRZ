//
//  SimpleBank.cpp
//
//  Created: 2014-09-18
//

#include "SimpleBank.h"

const string SimpleBank::DEFAULT_PASSWORD = "password";

SimpleBank::SimpleBank()
{
    //this->loggedOnUser_ = User{"Default"};
    //this->clientdb_ = ClientDB();
    double totalCash;
    this->clientdb_.loadFromFile(totalCash);
    
    if (!userExist("3307")) {
        clientdb_.addUser("3307", DEFAULT_PASSWORD, User::UserType::MGR);
        save();
    }
    
    if (!userExist("mnt")) {
        clientdb_.addUser("mnt", DEFAULT_PASSWORD, Client::UserType::MNT);
        save();
    }

    this->loggedOn_ = false;
    this->cashReserve_ = totalCash;
    //logger_.setAppName("Bank Server:");
    
    dbdel_.InitTables();
    //uids_ = NULL;
}

SimpleBank::~SimpleBank()
{
    //delete this->clientdb_;
    /*
    if (uids_ != NULL) {
        delete uids_;
    }
     */
}

bool SimpleBank::isLoggedOn()
{
    return this->loggedOn_;
}

bool SimpleBank::logon(string uid, string pass)
{
    if(!this->loggedOn_)
    {
        //log("Logging on as " + uid);
        DataEntry de;
        bool found = this->clientdb_.find(uid, de);
        
        if(found && (Utils::HashPassword(pass) == this->clientdb_.getUserPasswordHash(uid)))
        {
            this->loggedOn_ = true;
            this->loggedOnUser_ = ClientDB::makeUser(uid, (Client::UserType)de.utype);
            
            return true;
        }
    }
    return this->loggedOn_;
}

void SimpleBank::GetLoggedUID(string uname, int **uid)
{
    dbdel_.GetUID(uname, uid);
}

void SimpleBank::changePassword(const string & newPass)
{
    User u = loggedOnUser_;
    
}

void SimpleBank::updateClient(const string & oldUser, SB::Client newClient)
{
    clientdb_.updateClient(oldUser, newClient);
    clientdb_.saveToFile();
    double totalCash;
    clientdb_.loadFromFile(totalCash);
    this->cashReserve_ = totalCash;
}

void SimpleBank::logout()
{
    //log("Logging out as " + loggedOnUser_.getID());
    this->loggedOn_ = false;
    loggedOnUser_ = User();
}

void SimpleBank::newUser(string id, Client::UserType type)
{
    //log("Creating new user" + id);
    if (isLoggedOn() && loggedOnUser_.getUserType() == User::UserType::MGR) {
        User u = ClientDB::makeUser(id, type);
        
        this->clientdb_.addUser(u);
        this->clientdb_.saveToFile();
    }
}

void SimpleBank::openSavings(SB::Client & u)
{
    //log("Opening Savings Account for " + u.getID());
    if (loggedOnUser_.getUserType() == User::UserType::MGR) {
        u.openSavings();
    }
}

void SimpleBank::openChecking(SB::Client & u)
{
    //log("Opening Checking Account for "+ u.getID());
    if (loggedOnUser_.getUserType() == User::UserType::MGR) {
        u.openChecking();
    }
}

void SimpleBank::closeSavings(SB::Client & u)
{
    //log("Closing Savings Account for " + u.getID());
    if (loggedOnUser_.getUserType() == User::UserType::MGR) {
        u.closeSavings();
    }
}

void SimpleBank::closeChecking(SB::Client & u)
{
    //log("Closing Checking Account for " + u.getID());
    if (loggedOnUser_.getUserType() == User::UserType::MGR) {
        u.closeChecking();
    }
}

void SimpleBank::deleteUser(const string & id)
{
    //log("Delete User: " + id);
    this->clientdb_.removeUser(id);
}

void SimpleBank::print()
{
    cout << "Client DB\n";
    this->clientdb_.print();
    cout << "Logged On User: " << this->loggedOnUser_.getID() << "\n";
}

User SimpleBank::getUser(const string & uid)
{
    User u;
    clientdb_.getUser(uid, u);
    return u;
}

Client SimpleBank::getClient(const string & uid)
{
    Client c;
    clientdb_.getClient(uid, c);
    return c;
}

void SimpleBank::newUserDelegate(string uid, string password, Client::UserType utype)
{
    dbdel_.NewUser(uid, password, utype);
}

void SimpleBank::OpenAccountDel(int del_id, SB::AccountType at)
{
    dbdel_.OpenAccount(del_id,(int)at);
}

void SimpleBank::UpdateAccountBalance(int uid, int atype, double newBalance)
{
    dbdel_.UpdateAccountBalance(uid, atype, newBalance);
}