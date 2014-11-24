//
//  SimpleBank.cpp
//
//  Created: 2014-09-18
//

#include "SimpleBank.h"

SimpleBank::SimpleBank()
{
    //this->loggedOnUser_ = User{"Default"};
    //this->clientdb_ = ClientDB();
    double totalCash;
    this->clientdb_.loadFromFile(totalCash);
    dbdel_.InitTables();
    
    if (!userExist(DEFAULT_MANAGER)) {
        clientdb_.addUser(DEFAULT_MANAGER, DEFAULT_PASSWORD, User::UserType::MGR);
        save();
        dbdel_.NewUser(DEFAULT_MANAGER, DEFAULT_PASSWORD, User::UserType::MGR);
    }
    
    if (!userExist(DEFAULT_MAINTENANCE)) {
        clientdb_.addUser(DEFAULT_MAINTENANCE, DEFAULT_PASSWORD, Client::UserType::MNT);
        save();
        dbdel_.NewUser(DEFAULT_MAINTENANCE, DEFAULT_PASSWORD, User::UserType::MNT);
    }

    this->loggedOn_ = false;
    this->cashReserve_ = totalCash;
}

SimpleBank::~SimpleBank(){}

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

int SimpleBank::GetLoggedUID(string uname)
{
    return dbdel_.GetUID(uname);
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

void SimpleBank::UpdateAccountBalanceDel(int uid, int atype, double newBalance)
{
    dbdel_.UpdateAccountBalanceDel(uid, atype, newBalance);
}

vector<db_transaction_record> SimpleBank::GetTransactionRecords(int uid)
{
    return dbdel_.GetTransactionRecords(uid);
}

double SimpleBank::GetAccountBalance(int uid, int type)
{
    return dbdel_.GetAccountBalance(uid, type);
}

void SimpleBank::TriggerEndOfMonth()
{
    Logger c_logger("FailedCreditPayments.txt");
    c_logger.setAppName("SYS");
    
    vector<db_user_record_s> uids_to_process = dbdel_.GetEnabledCreditCustomers();
    auto it = uids_to_process.begin();
    while (it!=uids_to_process.end()) {
        db_user_record_s urs = *it++;
        int id = urs.u_id;
        string uname = urs.username;
        
        db_account_record checking_r = dbdel_.GetCheckingRecordForUser(id);
        db_credit_record credit_r = dbdel_.GetCreditRecordForUser(id);
        
        double checking_bal = checking_r.balance;
        double credit_bal = credit_r.balance;
        
        double obligate_amt;
        if (credit_r.credit_option == PAY_MIN_10) {
            obligate_amt = credit_bal * 0.10;
            
        }
        else if (credit_r.credit_option == PAY_FULL)
        {
            obligate_amt = credit_bal;
        }
        
        if (checking_bal >= obligate_amt) {
            //Enough money
            
            Client c = getClient(uname);
            double cRem;
            c.withdrawChecking(obligate_amt, cRem);
            updateClient(uname, c);
            
            UpdateAccountBalanceDel(id, ACCOUNT_TYPE_CHECKING, cRem);
            
            double newCreditBal = credit_bal - obligate_amt;
            
            if (credit_r.credit_option == PAY_MIN_10) {
                newCreditBal *= 1.02;
            }
            
            UpdateAccountBalanceDel(id, ACCOUNT_TYPE_CREDIT, newCreditBal);
            //Update to hide transactions
        }
        else
        {
            //Not enough money, freeze credit account
            dbdel_.SetAccountActivated(id, false);
            c_logger.logTrace("User: " + to_string(id) + " " + uname + " failed to pay credit balance of $" + to_string(credit_bal));
            c_logger.logTrace("Freezing credit account for: " + uname);
        }
    }
}


bool SimpleBank::payCreditOverDues(int uid)
{
    if (!dbdel_.IsUserCreditValid(loggedOnUser_.getID()) && dbdel_.GetAccountBalance(uid, ACCOUNT_TYPE_CREDIT) > 0) {
        db_account_record checking_r = dbdel_.GetCheckingRecordForUser(uid);
        db_credit_record credit_r = dbdel_.GetCreditRecordForUser(uid);
        
        double checking_bal = checking_r.balance;
        double credit_bal = credit_r.balance;
        
        if (checking_bal >= credit_bal) {
            
            Client c = getClient(loggedOnUser_.getID());
            double cRem;
            c.withdrawChecking(credit_bal, cRem);
            updateClient(loggedOnUser_.getID(), c);
            
            UpdateAccountBalanceDel(uid, ACCOUNT_TYPE_CHECKING, cRem);
            UpdateAccountBalanceDel(uid, ACCOUNT_TYPE_CREDIT, 0);
            dbdel_.SetAccountActivated(uid, true);
            return true;
        }
    }
    return false;
}